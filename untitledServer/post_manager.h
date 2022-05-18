#ifndef POST_MANAGER_H
#define POST_MANAGER_H

#include <QObject>
#include <map>
#include "server.h"
#include "userinfo.h"

class Post_Manager : public QObject
{
    Q_OBJECT
public:
    explicit Post_Manager(QObject *parent = nullptr);

    void write(uint64_t id, const std::string& msg);
    void accept_client(uint64_t id);
    void read_msg_from_client(uint64_t id, const char* ptr, uint64_t size);
private:
    void login(uint64_t id, const std::string& msg);
signals:
    void write_msg(uint64_t id, std::string msg);
//    void signal_accept_client(uint64_t);
//    void signal_read_msg_from_client(QString);
private:
    std::map<std::string, UserInfo> users_;
    std::map<uint64_t, UserInfo*> connected_users_;
    Server server_;

};

#endif // POST_MANAGER_H
