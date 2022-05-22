#include "server.h"
#include <fstream>
#include <charconv>
#include <filesystem>

Server::Server(std::function<void(uint64_t)> accept_handler, std::function<void(uint64_t, const char*, uint64_t)> read_handler)
    :acceptor_(context_), accept_handler_(accept_handler), read_handler_(read_handler)
{
    int port = 0;
    if(!std::filesystem::exists("host.conf")){
        port = 8001;
        std::ofstream out("host.conf");
        std::string str("8001");
        out.write(str.c_str(), str.size());
        out.close();
    }else{
        std::ifstream in("host.conf");
        if(in.is_open()){
            std::string str;
            std::getline(in, str);
            in.close();
            std::from_chars(str.data(), str.data()+str.size(), port);
        }
    }
    tcp::endpoint ep(tcp::endpoint(tcp::v4(), port));
    acceptor_.open(ep.protocol());
    acceptor_.bind(ep);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.listen();
    do_accept();
    std::thread tmp([this](){context_.run();});
    context_thread_ = std::move(tmp);
}

Server::~Server()
{
    context_.stop();
    context_thread_.join();
}

void Server::write(uint64_t id, const char* ptr, uint64_t size)
{
    net_msg msg(ptr, size);
    bool in_progress = !write_msgs_.at(id).empty();
    write_msgs_.at(id).push_back(msg);
    if(!in_progress){
        do_write(id);
    }
}

void Server::do_write(uint64_t id)
{
    boost::asio::async_write(sockets_.at(id), boost::asio::buffer(write_msgs_.at(id).front().data(), write_msgs_.at(id).front().size()),
        [this, id](const boost::system::error_code& ec, std::size_t /*length*/){
            if(!ec){
                write_msgs_.at(id).pop_front();
                if(!write_msgs_.at(id).empty())
                    do_write(id);
            }else{
                uint8_t code = 127;
                read_handler_(id, (char*)&code, 1);
            }
        });
}

void Server::do_read_header(uint64_t id)
{
    boost::asio::async_read(sockets_.at(id), boost::asio::buffer(read_msgs_.at(id).data(), net_msg::header_size_),
        [this, id](const boost::system::error_code& ec, std::size_t /*length*/){
            if(!ec){
                read_msgs_.at(id).allocate_size_for_body();
                do_read_body(id);
            }else{
                uint8_t code = 127;
                read_handler_(id, (char*)&code, 1);
            }

    });
}

void Server::do_read_body(uint64_t id)
{
    boost::asio::async_read(sockets_.at(id), boost::asio::buffer(read_msgs_.at(id).body_data(), read_msgs_.at(id).body_size()),
        [this, id](const boost::system::error_code& ec, std::size_t /*length*/){
            if(!ec){
                read_handler_(id, read_msgs_.at(id).body_data(), read_msgs_.at(id).body_size());
                do_read_header(id);
            }else{
                uint8_t code = 127;
                read_handler_(id, (char*)&code, 1);
            }
    });
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](const boost::system::error_code& ec, tcp::socket socket){
            if(!ec){
                static uint64_t id = 1;
                sockets_.emplace(id, std::move(socket));
                write_msgs_.emplace(id, std::deque<net_msg>());
                read_msgs_.emplace(id, net_msg());
                accept_handler_(id);
                do_read_header(id);
                ++id;
            }
            do_accept();
        });
}
