#ifndef POSTRECIPIENT_H
#define POSTRECIPIENT_H

#include <QObject>
#include "client.h"
#include "userinfo.h"
#include "roominfo.h"

class MessageStream;

class PostRecipient : public QObject
{
    Q_OBJECT

    enum{
        MSG_CODE_CONNECT,
        MSG_CODE_LOGIN,
        MSG_CODE_REGISTRATION,
        MSG_CODE_LOGOUT,
        MSG_CODE_MESSAGE,
        MSG_CODE_CHATS_LIST,
        MSG_CODE_FRIENDS_LIST,
        MSG_CODE_USERS_LIST,
        MSG_CODE_DELETE_FRIEND,
        MSG_CODE_ADD_FRIEND,
        MSG_CODE_CREATE_ROOM,
        MSG_CODE_OPEN_ROOM,
        MSG_CODE_CREATE_IMAGE,
        MSG_CODE_CHANGE_INFO,
        MSG_CODE_LOAD_IMAGE,
        MSG_CODE_QUIT = 127
    };

public:
    explicit PostRecipient(QObject *parent = nullptr);


    const UserInfo& getUserInfo()const;

    void write(const std::string& msg);
    void decode(const char* ptr, uint64_t size);
    void connect_to_server();
    const std::map<uint64_t, std::string>& user_friends()const;
    const std::map<uint64_t, std::string>& user_rooms()const;

    void login_request(const std::string& login, const std::string& password);
    void registration_request(const UserInfo& info);
    void logout_request();
    void users_list_request();
    void delete_friend_request(uint64_t friend_id);
    void add_friend_request(uint64_t friend_id);
    void create_room_request(const std::string& name, const std::vector<uint64_t>& members_id);
    void open_room_request(uint64_t room_id);
    void send_message_request(uint64_t room_id, const std::string& msg);
    void change_profile_info_request(const UserInfo& info);

    void create_image_request(uint64_t size, const uchar* data, uint64_t width, uint64_t height, uint64_t format);
    void load_image_request();
private:
    void connect_answer(const MessageStream& stream);
    void login_answer(const MessageStream& stream);
    void registration_answer(const MessageStream& stream);
    void logout_answer(const MessageStream& stream);
    void users_list_answer(const MessageStream& stream);
    void delete_friend_answer(const MessageStream& stream);
    void add_friend_answer(const MessageStream& stream);
    void create_room_answer(const MessageStream& stream);
    void open_room_answer(const MessageStream& stream);
    void send_message_answer(const MessageStream& stream);
    void change_profile_info_answer(const MessageStream& stream);
    void create_image(const MessageStream& stream);
    void load_image(const MessageStream& stream);
signals:
    void signal_write_msg(std::string msg);
    void signal_login_answer(bool result);
    void signal_registration_answer(bool result);
    void signal_logout_answer(bool result);
    void signal_friends_list_answer(std::map<uint64_t, std::string>);
    void signal_users_list_answer(std::deque<std::pair<uint64_t, std::string>>);
    void signal_delete_friend_answer(uint64_t friend_id);
    void signal_add_friend_answer(uint64_t friend_id, std::string name);
    void signal_add_room_answer(uint64_t room_id, std::string room_name);
    void signal_open_room_answer(uint64_t room_id, RoomInfo room_info);
    void signal_send_message_answer(uint64_t room_id, std::string user_name, std::string message);
    void signal_change_profile_info_answer(bool result);
    void signal_lost_connection();
    void signal_load_image(std::string, uint64_t, uint64_t, uint64_t);
private:
     Client client_;
     UserInfo user_;
     uint64_t current_room_ = 0;
};

#endif // POSTRECIPIENT_H
