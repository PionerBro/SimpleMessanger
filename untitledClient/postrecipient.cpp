#include "postrecipient.h"
#include <functional>
#include "messagestream.h"
#include <mutex>

#include <QDebug>

using namespace std::placeholders;


PostRecipient::PostRecipient(QObject *parent)
    : QObject{parent}, client_(std::bind(&PostRecipient::connect_to_server, this),
                                     std::bind(&PostRecipient::decode, this, _1, _2)),
      current_room_(0)
{

}

const UserInfo &PostRecipient::getUserInfo() const
{
    return user_;
}

void PostRecipient::write(const std::string &msg)
{
    client_.write(msg.data(), msg.size());
}

void PostRecipient::connect_to_server()
{

}

const std::map<uint64_t, std::string>& PostRecipient::user_friends() const
{
    return user_.friends();
}

const std::map<uint64_t, std::string> &PostRecipient::user_rooms() const
{
    return user_.rooms();
}

void PostRecipient::login_request(const std::string &login, const std::string &password)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_LOGIN)<<login<<password;
    client_.write(out.data(), out.size());
}

void PostRecipient::registration_request(const UserInfo &info)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_REGISTRATION)<<info;
    client_.write(out.data(), out.size());
}

void PostRecipient::logout_request()
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_LOGOUT);
    client_.write(out.data(), out.size());
}

void PostRecipient::users_list_request()
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_USERS_LIST);
    client_.write(out.data(), out.size());
}

void PostRecipient::delete_friend_request(uint64_t friend_id)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_DELETE_FRIEND)<<friend_id;
    client_.write(out.data(), out.size());
}

void PostRecipient::add_friend_request(uint64_t friend_id)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_ADD_FRIEND)<<friend_id;
    client_.write(out.data(), out.size());
}

void PostRecipient::create_room_request(const std::string &name, const std::vector<uint64_t> &members_id)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_CREATE_ROOM)<<name<<static_cast<uint64_t>(members_id.size());
    for(const auto& member:members_id)
        out<<member;
    client_.write(out.data(), out.size());
}

void PostRecipient::open_room_request(uint64_t room_id)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_OPEN_ROOM)<<room_id;
    client_.write(out.data(), out.size());
}

void PostRecipient::send_message_request(uint64_t room_id, const std::string &msg)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_MESSAGE)<<room_id<<msg;
    client_.write(out.data(), out.size());
}

void PostRecipient::change_profile_info_request(const UserInfo &info)
{
    MessageStream out;
    out<<static_cast<uint8_t>(MSG_CODE_CHANGE_INFO)<<info;
    client_.write(out.data(), out.size());
}

void PostRecipient::decode(const char *ptr, uint64_t size)
{
    MessageStream stream(ptr, size);
    uint8_t code = 0;
    stream>>code;

    switch (code) {
    case MSG_CODE_CONNECT:
        connect_answer(stream);
        break;
    case MSG_CODE_LOGIN:
        login_answer(stream);
        break;
    case MSG_CODE_REGISTRATION:
        registration_answer(stream);
        break;
    case MSG_CODE_LOGOUT:
        logout_answer(stream);
        break;
    case MSG_CODE_MESSAGE:
        send_message_answer(stream);
        break;
    case MSG_CODE_CHATS_LIST:
        break;
    case MSG_CODE_FRIENDS_LIST:
        break;
    case MSG_CODE_USERS_LIST:
        users_list_answer(stream);
        break;
    case MSG_CODE_DELETE_FRIEND:
        delete_friend_answer(stream);
        break;
    case MSG_CODE_ADD_FRIEND:
        add_friend_answer(stream);
        break;
    case MSG_CODE_CREATE_ROOM:
        create_room_answer(stream);
        break;
    case MSG_CODE_OPEN_ROOM:
        open_room_answer(stream);
        break;
    case MSG_CODE_CHANGE_INFO:
        change_profile_info_answer(stream);
        break;
    case MSG_CODE_QUIT:
        emit signal_lost_connection();
        break;
    default:
        break;
    }
}

void PostRecipient::connect_answer(const MessageStream &stream)
{
    std::string str;
    stream>>str;
    emit signal_write_msg(str);
}

void PostRecipient::login_answer(const MessageStream &stream)
{
    uint8_t success = 1;
    stream>>success;
    if(success){
        emit signal_write_msg("Logining failed");
        emit signal_login_answer(false);
        return;
    }
    stream>>user_;
    emit signal_write_msg("Logining Ok! User: " + user_.name() + " " + user_.surname());
    emit signal_friends_list_answer(user_.friends());
    emit signal_login_answer(true);
}

void PostRecipient::registration_answer(const MessageStream &stream)
{
    uint8_t success = 1;
    stream>>success;
    if(success){
        emit signal_write_msg("Registration failed");
        emit signal_registration_answer(false);
        return;
    }
    emit signal_write_msg("Registration Ok");
    emit signal_registration_answer(true);
}

void PostRecipient::logout_answer(const MessageStream &stream)
{
    uint8_t success = 1;
    stream>>success;
    if(success){
        emit signal_write_msg("Logout failed");
        emit signal_logout_answer(false);
        return;
    }
    emit signal_write_msg("Logout Ok");
    emit signal_logout_answer(true);
}

void PostRecipient::users_list_answer(const MessageStream &stream)
{
    uint8_t success = 1;
    stream>>success;
    if(success){
        emit signal_write_msg("Users_list failed");
        return;
    }
    uint64_t size = 0;
    stream >> size;
    std::deque<std::pair<uint64_t, std::string>> users_;
    std::string user_list = "";
    for(uint64_t i = 0; i < size; ++i){
        uint64_t id = 0;
        std::string name;
        std::string surname;
        stream>>id>>name>>surname;
        if(user_.friends().find(id)==user_.friends().end() && user_.name() != name)
            users_.emplace_back(id, name+" "+surname);
        user_list.append(std::to_string(id) + " " + name + " " +surname+", ");
    }
    emit signal_users_list_answer(users_);
    emit signal_write_msg("Users_list Ok: " + user_list);
}

void PostRecipient::delete_friend_answer(const MessageStream &stream)
{
    uint8_t succes = 1;
    stream>>succes;
    if(succes){
        emit signal_write_msg("Delete friend failed");
        return;
    }
    uint64_t friend_id = 0;
    stream>>friend_id;
    user_.erase_friend(friend_id);
    emit signal_delete_friend_answer(friend_id);
    emit signal_write_msg("Delete friend Ok");
}

void PostRecipient::add_friend_answer(const MessageStream &stream)
{
    uint8_t succes = 1;
    stream>>succes;
    if(succes){
        emit signal_write_msg("Add friend failed");
        return;
    }
    uint64_t friend_id = 0;
    std::string name;
    stream>>friend_id>>name;
    user_.add_friend(friend_id, name);
    emit signal_add_friend_answer(friend_id, name);
    emit signal_write_msg("Add friend Ok");
}

void PostRecipient::create_room_answer(const MessageStream &stream)
{
    uint8_t succes = 1;
    stream>>succes;
    if(succes){
        emit signal_write_msg("create room failed");
        return;
    }
    uint64_t room_id = 0;
    std::string room_name;
    stream>>room_id>>room_name;
    user_.add_room(room_id, room_name);
    emit signal_add_room_answer(room_id, room_name);
    emit signal_write_msg("create room Ok");
}

void PostRecipient::open_room_answer(const MessageStream &stream)
{
    uint8_t succes = 1;
    stream>>succes;
    if(succes){
        emit signal_write_msg("open room failed");
        return;
    }
    uint64_t room_id = 0;
    RoomInfo info;
    stream>>room_id>>info;
    current_room_ = room_id;
    emit signal_open_room_answer(room_id, info);
    emit signal_write_msg("open room Ok");
}

void PostRecipient::send_message_answer(const MessageStream &stream)
{
    uint8_t succes = 1;
    stream>>succes;
    if(succes){
        emit signal_write_msg("send message failed");
        return;
    }
    uint64_t room_id = 0;
    stream>>room_id;
    if(current_room_ != room_id)
        return;
    std::string user_name;
    std::string message;
    stream >> user_name>>message;
    emit signal_send_message_answer(room_id, user_name, message);
    emit signal_write_msg("send message Ok");
}

void PostRecipient::change_profile_info_answer(const MessageStream &stream)
{
    uint8_t succes = 1;
    stream>>succes;
    if(succes){
        emit signal_change_profile_info_answer(false);
        emit signal_write_msg("send message failed");
        return;
    }
    UserInfo info;
    stream>>info;
    user_.set_image(info.image());
    user_.set_name(info.name());
    user_.set_surname(info.surname());
    emit signal_change_profile_info_answer(true);
    emit signal_write_msg("send message Ok");
}
