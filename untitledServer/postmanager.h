#ifndef POSTMANAGER_H
#define POSTMANAGER_H

#include <QObject>
#include <map>
#include "server.h"
#include "userinfo.h"
#include "roominfo.h"


class MessageStream;

class PostManager : public QObject
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
        MSG_CODE_CHANGE_INFO,
        MSG_CODE_QUIT = 127
    };
public:
    PostManager(QObject* parent = nullptr);

    void write(uint64_t id, const std::string& msg);
    void accept_client(uint64_t id);
    void read_msg_from_client(uint64_t id, const char* ptr, uint64_t size);
private:
    void login(uint64_t id, const MessageStream& stream);
    void registration(uint64_t id, const MessageStream& stream);
    void logout(uint64_t id);
    void users_list(uint64_t id);
    void delete_friend(uint64_t id, const MessageStream& stream);
    void add_friend(uint64_t id, const MessageStream& stream);
    void create_new_room(uint64_t id, const MessageStream& stream);
    void open_room(uint64_t id, const MessageStream& stream);
    void receive_message(uint64_t id, const MessageStream& stream);
    void quit_user(uint64_t connected_id);
    void change_user_info(uint64_t id, const MessageStream& stream);

    bool load_start_data();
    bool load_accounts_data();
    bool load_rooms_data();
    bool load_accounts(const std::vector<uint64_t>& vec_id);
    bool load_rooms(const std::vector<uint64_t>& vec_id);
    bool create_account(const UserInfo& info);
    bool create_room(uint64_t& room_id, const RoomInfo& info);
    bool notify_about_create_room(uint64_t& room_id, const RoomInfo& info);
    bool update_account_content(uint64_t id);
    bool update_room_content(uint64_t room_id);
signals:
    void write_msg(uint64_t id, std::string msg);
private:
    std::map<std::string, uint64_t> accounts_;
    std::map<uint64_t, UserInfo> users_;
    std::map<uint64_t, RoomInfo> rooms_;
    std::map<uint64_t, UserInfo*> connected_users_;
    std::map<uint64_t, uint64_t> logined_users_;//user_id, connected_id
    Server server_;
    std::string s;
    uint64_t width;
    uint64_t height;
    uint64_t format;
};

#endif // POSTMANAGER_H
