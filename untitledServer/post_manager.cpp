#include "post_manager.h"

#include <functional>

using namespace std::placeholders;

Post_Manager::Post_Manager(QObject *parent)
    : QObject{parent},
      server_(std::bind(&Post_Manager::accept_client, this, _1),
                                    std::bind(&Post_Manager::read_msg_from_client,this,_1,_2,_3))
{

}

void Post_Manager::write(uint64_t id, const std::string &msg)
{
    server_.write(id, msg.data(), msg.size());
}

void Post_Manager::accept_client(uint64_t id)
{
    connected_users_.emplace(id, nullptr);
    write(id, "You connected to server with id = " + std::to_string(id));
    emit write_msg(id, "Accept client!");
}

void Post_Manager::read_msg_from_client(uint64_t id, const char *ptr, uint64_t size)
{
    uint8_t code = 0;
    memcpy((char*)&code, ptr, sizeof(uint8_t));
    std::string msg(ptr+1, size-1);
    switch (code) {
    case 1:
    {
        login(id, msg);
        break;
    }
    default:
        break;
    }
}

void Post_Manager::login(uint64_t id, const std::string &msg)
{
    UserInfo& potencial = users_.at(msg);
    if(potencial.password() == "12345"){
        connected_users_.at(id) = &potencial;
        std::string str = "  "+ potencial.login() + potencial.name() + potencial.surname();
        str[0] = 1;
        str[1] = 0;
        write(id, str);
        emit write_msg(id, "Login Ok");
    }else{
        std::string str = "  ";
        str[0]= 1;
        str[1]= 1;
        write(id, str);
        emit write_msg(id, "Login failed");
    }
}
