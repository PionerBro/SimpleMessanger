#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <thread>
#include <deque>
#include <functional>

#include "net_msg.h"

using boost::asio::ip::tcp;

class Client
{
public:
    Client(std::function<void()> connect_handler, std::function<void(const char*, uint64_t)> read_handler);
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    ~Client();



    void write(const char* ptr, uint64_t size);
private:
    void do_connect();
    void do_read_header();
    void do_read_body();
    void do_write();


private:
    boost::asio::io_context context_;
    std::thread context_thread_;
    tcp::socket socket_;
    net_msg read_msg_;
    std::deque<net_msg> write_msgs_;

    std::function<void()> connect_handler_;
    std::function<void(const char*, uint64_t)> read_handler_;
};

#endif // CLIENT_H
