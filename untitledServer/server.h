#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <thread>
#include <map>
#include <deque>
#include "net_msg.h"

#include <functional>

using boost::asio::ip::tcp;

class Server
{
public:
    Server(std::function<void(uint64_t)> accept_handler, std::function<void(uint64_t, const char*, uint64_t)> read_handler);
    ~Server();

    void write(uint64_t id, const char* ptr, uint64_t size);
private:
    void do_write(uint64_t id);
    void do_read_header(uint64_t id);
    void do_read_body(uint64_t id);
    void do_accept();
private:
    boost::asio::io_context context_;
    std::thread context_thread_;
    tcp::acceptor acceptor_;
    std::map<uint64_t, std::deque<net_msg>> write_msgs_;
    std::map<uint64_t, tcp::socket> sockets_;
    std::map<uint64_t, net_msg> read_msgs_;

    std::function<void(uint64_t)> accept_handler_;
    std::function<void(uint64_t, const char*, uint64_t)> read_handler_;
};

#endif // SERVER_H
