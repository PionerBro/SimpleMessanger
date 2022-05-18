#include "client.h"
#include <fstream>
#include <string>

Client::Client(std::function<void()> connect_handler, std::function<void(const char*, uint64_t)> read_handler):
    socket_(context_), connect_handler_(connect_handler), read_handler_(read_handler)
{
    do_connect();
    std::thread tmp([this](){ context_.run();});
    context_thread_ = std::move(tmp);
}

Client::~Client(){
    context_.stop();
    context_thread_.join();
}

void Client::write(const char* ptr, uint64_t size)
{
    net_msg msg(ptr, size);
    boost::asio::post(context_, [this, msg](){
        bool in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if(!in_progress)
            do_write();
    });
}

void Client::do_connect()
{
    std::string address;
    std::string port;
    std::ifstream in("host.conf");
    if(in.is_open()){
        std::getline(in, address);
        std::getline(in, port);
        in.close();
    }
    tcp::resolver resolver(context_);
    auto endpoints = resolver.resolve({address,port});
    boost::asio::async_connect(socket_, endpoints,
        [this](const boost::system::error_code& ec, tcp::endpoint /*ep*/){
            if(!ec){
                connect_handler_();
                do_read_header();
            }else{
                uint8_t code = 127;
                read_handler_((char*)&code, 1);
            }
        });

}

void Client::do_read_header()
{
    boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), net_msg::header_size_),
        [this](const boost::system::error_code& ec, std::size_t /*length*/){
            if(!ec){
                read_msg_.allocate_size_for_body();
                do_read_body();
            }else{
                uint8_t code = 127;
                read_handler_((char*)&code, 1);
            }
        });
}

void Client::do_read_body()
{
    boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body_data(), read_msg_.body_size()),
        [this](const boost::system::error_code& ec, std::size_t /*length*/){
            if(!ec){
                read_handler_(read_msg_.body_data(), read_msg_.body_size());
                do_read_header();
            }else{
                uint8_t code = 127;
                read_handler_((char*)&code, 1);
            }
        });
}

void Client::do_write()
{
    boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().size()),
        [this](const boost::system::error_code& ec, std::size_t /*length*/){
            if(!ec){
                write_msgs_.pop_front();
                if(!write_msgs_.empty())
                    do_write();
            }else{
                uint8_t code = 127;
                read_handler_((char*)&code, 1);
            }
        });
}

