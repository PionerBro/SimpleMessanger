#include "postmanager.h"
#include "messagestream.h"

#include <functional>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <charconv>

#include <QDebug>

using namespace std::placeholders;

PostManager::PostManager(QObject* parent) : QObject(parent),
    server_(std::bind(&PostManager::accept_client, this, _1),
                                  std::bind(&PostManager::read_msg_from_client,this,_1,_2,_3))
{
    if(!load_start_data())
        emit write_msg(0, "Не удалось прочитать файлы users.info и rooms.info");
}

void PostManager::write(uint64_t id, const std::string &msg)
{
    server_.write(id, msg.data(), msg.size());
}

void PostManager::accept_client(uint64_t id)
{
    connected_users_.emplace(id, nullptr);
    write(id, "You connected to server with id = " + std::to_string(id));
    emit write_msg(id, "Accept client!");
}

void PostManager::read_msg_from_client(uint64_t id, const char *ptr, uint64_t size)
{
    MessageStream stream(ptr, size);
    uint8_t code = 0;
    stream>>code;
    switch (code) {
    case MSG_CODE_LOGIN:
    {
        login(id, stream);
        break;
    }
    case MSG_CODE_REGISTRATION:
    {
        registration(id, stream);
        break;
    }
    case MSG_CODE_LOGOUT:
    {
        logout(id);
        break;
    }
    case MSG_CODE_MESSAGE:
        receive_message(id, stream);
        break;
    case MSG_CODE_CHATS_LIST:
        break;
    case MSG_CODE_FRIENDS_LIST:
        break;
    case MSG_CODE_USERS_LIST:
        users_list(id);
        break;
    case MSG_CODE_DELETE_FRIEND:
        delete_friend(id, stream);
        break;
    case MSG_CODE_ADD_FRIEND:
        add_friend(id, stream);
        break;
    case MSG_CODE_CREATE_ROOM:
        create_new_room(id, stream);
        break;
    case MSG_CODE_OPEN_ROOM:
        open_room(id, stream);
        break;
    case MSG_CODE_CHANGE_INFO:
        change_user_info(id, stream);
        break;
    case MSG_CODE_QUIT:
        quit_user(id);
        break;
    default:
        break;
    }
}

void PostManager::login(uint64_t id, const MessageStream &stream)
{
    std::string login;
    std::string password;
    stream>>login>>password;
    auto it = accounts_.find(login);
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_LOGIN);
    if(it!=accounts_.end() && users_.at(it->second).password() == password){
        connected_users_.at(id) = &users_.at(it->second);
        logined_users_.emplace(it->second, id);
        answer<<uint8_t(0)<<users_.at(it->second);
        write(id, answer.row_data());
        emit write_msg(id, "Login Ok");
    }else{
        answer<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Login failed");
    }
}

void PostManager::registration(uint64_t id, const MessageStream &stream)
{
    UserInfo info;
    stream>>info;
    auto it = accounts_.find(info.login());
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_REGISTRATION);
    if(it==accounts_.end() && create_account(info)){
        answer<<uint8_t(0);
        write(id, answer.row_data());
        emit write_msg(id, "Registration Ok");
    }else{
        answer<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Registration failed");
    }

}

void PostManager::logout(uint64_t id)
{
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_LOGOUT);
    auto& self_info = connected_users_.at(id);
    if(self_info){
        logined_users_.erase(accounts_.at(self_info->login()));
        self_info = nullptr;
        answer<<uint8_t(0);
        write(id, answer.row_data());
        emit write_msg(id, "Logout Ok");
    }else{
        answer<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Logout failed");
    }
}

void PostManager::users_list(uint64_t id)
{
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_USERS_LIST)<<uint8_t(0)<<uint64_t(users_.size());
    for(const auto& user:users_)
        answer<<user.first<<user.second.name()<<user.second.surname();
    write(id, answer.row_data());
    emit write_msg(id, "User_list Ok");
}

void PostManager::delete_friend(uint64_t id, const MessageStream &stream)
{
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_DELETE_FRIEND);
    uint64_t friend_id = 0;
    stream>>friend_id;
    auto friend_it_in_users = users_.find(friend_id);
    auto self_info = connected_users_.at(id);
    if(friend_it_in_users!=users_.end() && (self_info->friends().find(friend_id) != self_info->friends().end())){
        uint64_t self_id = accounts_.at(self_info->login());
        self_info->erase_friend(friend_id);
        friend_it_in_users->second.erase_friend(self_id);
        update_account_content(self_id);
        update_account_content(friend_id);
        answer<<uint8_t(0)<<friend_id;
        write(id, answer.row_data());
        auto friend_it_in_log_users = logined_users_.find(friend_id);
        if(friend_it_in_log_users != logined_users_.end()){
            MessageStream answer1;
            answer1<<uint8_t(MSG_CODE_DELETE_FRIEND)<<uint8_t(0)<<self_id;
            write(friend_it_in_log_users->second, answer1.row_data());
        }
        emit write_msg(id, "Add delete Ok");
    }else{
        answer<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Add delete failed");
    }
}

void PostManager::add_friend(uint64_t id, const MessageStream &stream)
{
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_ADD_FRIEND);
    uint64_t friend_id = 0;
    stream>>friend_id;
    auto friend_it_in_users = users_.find(friend_id);
    auto self_info = connected_users_.at(id);
    if(friend_it_in_users!=users_.end() && (self_info->friends().find(friend_id) == self_info->friends().end())){
        uint64_t self_id = accounts_.at(self_info->login());
        std::string friend_name = friend_it_in_users->second.name() + " " + friend_it_in_users->second.surname();
        std::string self_name = self_info->name() + " " + self_info->surname();
        self_info->add_friend(friend_id, friend_name);
        friend_it_in_users->second.add_friend(self_id, self_name);
        update_account_content(self_id);
        update_account_content(friend_id);
        answer<<uint8_t(0)<<friend_id<<friend_name;
        write(id, answer.row_data());
        auto friend_it_in_log_users = logined_users_.find(friend_id);
        if(friend_it_in_log_users != logined_users_.end()){
            MessageStream answer1;
            answer1<<uint8_t(MSG_CODE_ADD_FRIEND)<<uint8_t(0)<<self_id<<self_name;
            write(friend_it_in_log_users->second, answer1.row_data());
        }
        emit write_msg(id, "Add Friend Ok");
    }else{
        answer<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Add Friend failed");
    }
}

void PostManager::create_new_room(uint64_t id, const MessageStream &stream)
{
    std::string name;
    uint64_t members_size;
    stream>>name>>members_size;
    RoomInfo info;
    info.set_name(name);
    const auto self_info = connected_users_.at(id);
    info.add_members(accounts_.at(self_info->login()), self_info->name());
    ++members_size;
    for(std::size_t i = 1; i < members_size; ++i){
        uint64_t member_id = 0;
        stream>>member_id;
        info.add_members(member_id, users_.at(member_id).name());
    }
    uint64_t room_id = 0;
    if(create_room(room_id, info) && notify_about_create_room(room_id, info)){
        MessageStream answer;
        answer<<uint8_t(MSG_CODE_CREATE_ROOM)<<uint8_t(0)<<room_id<<info.name();
        for(const auto& member : info.members()){
            auto it = logined_users_.find(member.first);
            if(it!=logined_users_.end()){
                write(it->second, answer.row_data());
            }
        }
        emit write_msg(id, "Create room Ok");
    }else{
        MessageStream answer;
        answer<<uint8_t(MSG_CODE_CREATE_ROOM)<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Create room failed");
    }
}

void PostManager::open_room(uint64_t id, const MessageStream &stream)
{
    uint64_t room_id = 0;
    stream>>room_id;
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_OPEN_ROOM);
    auto it = rooms_.find(room_id);
    if(it != rooms_.end()){
        answer<<uint8_t(0)<<room_id<<it->second;
        write(id, answer.row_data());
        emit write_msg(id, "Open room Ok");
    }else{
        answer<<uint8_t(1);
        write(id, answer.row_data());
        emit write_msg(id, "Open room failed");
    }
}

void PostManager::receive_message(uint64_t id, const MessageStream &stream)
{
    uint64_t room_id = 0;
    std::string msg;
    stream>>room_id>>msg;

    auto it = rooms_.find(room_id);
    if(it != rooms_.end()){
        auto self_info = connected_users_.at(id);
        std::string self_name = self_info->name() + " " + self_info->surname();
        it->second.add_message(self_name, msg);
        RoomInfo info = it->second;
        if(update_room_content(room_id)){
            MessageStream answer;
            answer<<uint8_t(MSG_CODE_MESSAGE)<<uint8_t(0)<<room_id<<self_name<<msg;
            for(const auto& member:it->second.members()){
                auto it = logined_users_.find(member.first);
                if(it!=logined_users_.end())
                    write(it->second, answer.row_data());
            }
            emit write_msg(id, "Send message Ok");
            return;
        }
    }
    MessageStream answer;
    answer<<uint8_t(1);
    write(id, answer.row_data());
    emit write_msg(id, "Send msg failed");
}

void PostManager::quit_user(uint64_t connected_id)
{
    auto& info = connected_users_.at(connected_id);
    if(info){
        uint64_t user_id = accounts_.at(info->login());
        logined_users_.erase(user_id);
    }
    connected_users_.erase(connected_id);
}

void PostManager::change_user_info(uint64_t id, const MessageStream &stream)
{
    UserInfo info;
    stream>>info;
    uint64_t user_id = logined_users_.at(id);
    users_.at(user_id).set_name(info.name());
    users_.at(user_id).set_surname(info.surname());
    users_.at(user_id).set_image(info.image());
    MessageStream answer;
    answer<<uint8_t(MSG_CODE_CHANGE_INFO);
    if(update_account_content(user_id)){
        answer<<uint8_t(0)<<info;
        emit write_msg(id, "Change info Ok");
    }else{
        answer<<uint8_t(1);
        emit write_msg(id, "Chnage info failed");
    }
    write(id, answer.row_data());
}

bool PostManager::load_start_data()
{
    if(load_accounts_data() && load_rooms_data())
        return true;
    return false;
}

bool PostManager::load_accounts_data()
{
    std::string filename = "users.info";
    if(!std::filesystem::exists(filename)){
        std::ofstream out(filename);
        if(out.is_open()){
            out.close();
            return true;
        }
        return false;
    }
    std::ifstream in(filename, std::ios::binary);
    if(in.is_open()){
        std::stringstream s_stream;
        s_stream<<in.rdbuf();
        MessageStream stream(s_stream.str());
        in.close();
        std::vector<uint64_t> accounts;
        while(!stream.eof()){
            uint64_t user_id = 0;
            stream>>user_id;
            accounts.push_back(user_id);
        }
        if(load_accounts(accounts))
            return true;
    }
    return false;
}

bool PostManager::load_rooms_data()
{
    std::string filename = "rooms.info";
    if(!std::filesystem::exists(filename)){
        std::ofstream out(filename);
        if(out.is_open()){
            out.close();
            return true;
        }
        return false;
    }
    std::ifstream in(filename, std::ios::binary);
    if(in.is_open()){
        std::stringstream s_stream;
        s_stream<<in.rdbuf();
        MessageStream stream(s_stream.str());
        in.close();
        std::vector<uint64_t> rooms;
        while(!stream.eof()){
            uint64_t room_id = 0;
            stream>>room_id;
            rooms.push_back(room_id);
        }
        if(load_rooms(rooms))
            return true;
    }
    return false;
}

bool PostManager::load_accounts(const std::vector<uint64_t> &vec_id)
{
    std::ifstream in;
    for(const auto id : vec_id){
        std::string filename = std::to_string(id) + ".acc";
        in.open(filename, std::ios::binary);
        if(!in.is_open())
            return false;
        std::stringstream s_stream;
        s_stream<<in.rdbuf();
        MessageStream stream(s_stream.str());
        in.close();
        UserInfo info;
        stream>>info;
        users_.emplace(id, info);
        accounts_.emplace(info.login(), id);
    }
    return true;
}

bool PostManager::load_rooms(const std::vector<uint64_t> &vec_id)
{
    std::ifstream in;
    for(const auto id : vec_id){
        std::string  filename = std::to_string(id) + ".room";
        in.open(filename, std::ios::binary);
        if(!in.is_open())
            return false;
        std::stringstream s_stream;
        s_stream<<in.rdbuf();
        MessageStream stream(s_stream.str());
        in.close();
        RoomInfo info;
        stream>>info;
        rooms_.emplace(id, info);
    }
    return true;
}

bool PostManager::create_account(const UserInfo &info)
{
    uint64_t id = 1;
    if(!users_.empty())
        id = users_.crbegin()->first+1;
    std::ofstream out;
    out.open("users.info", std::ios::app|std::ios::binary);
    if(!out.is_open())
        return false;
    MessageStream stream;
    stream<<id;
    out.write(stream.data(),stream.size());
    out.close();
    std::string filename = std::to_string(id) + ".acc";
    out.open(filename, std::ios::binary);
    if(!out.is_open())
        return false;
    stream.clear();
    stream<<info;
    out.write(stream.data(), stream.size());
    out.close();
    users_.emplace(id, info);
    accounts_.emplace(info.login(), id);
    return true;
}

bool PostManager::create_room(uint64_t& id, const RoomInfo& info)
{
    id = 1;
    if(!rooms_.empty())
        id = rooms_.crbegin()->first+1;
    std::ofstream out;
    out.open("rooms.info", std::ios::app|std::ios::binary);
    if(!out.is_open())
        return false;
    MessageStream stream;
    stream<<id;
    out.write(stream.data(),stream.size());
    out.close();
    std::string filename = std::to_string(id) + ".room";
    out.open(filename, std::ios::binary);
    if(!out.is_open())
        return false;
    stream.clear();
    stream<<info;
    out.write(stream.data(), stream.size());
    out.close();
    rooms_.emplace(id, info);
    return true;
}

bool PostManager::notify_about_create_room(uint64_t &room_id, const RoomInfo &info)
{
    for(const auto& member:info.members()){
        users_.at(member.first).add_room(room_id, info.name());
        if(!update_account_content(member.first))
            return false;
    }
    return true;
}

bool PostManager::update_account_content(uint64_t id)
{
    MessageStream stream;
    stream<<users_.at(id);
    std::ofstream out;
    std::string filename = std::to_string(id) + ".acc";
    out.open(filename, std::ios::binary | std::ios::trunc);
    if(!out.is_open())
        return false;
    out.write(stream.data(), stream.size());
    out.close();
    return true;
}

bool PostManager::update_room_content(uint64_t room_id)
{
    MessageStream stream;
    RoomInfo info = rooms_.at(room_id);
    stream<<info;
    std::ofstream out;
    std::string filename = std::to_string(room_id) + ".room";
    out.open(filename, std::ios::binary | std::ios::trunc);
    if(!out.is_open())
        return false;
    out.write(stream.data(), stream.size());
    out.close();
    return true;
}
