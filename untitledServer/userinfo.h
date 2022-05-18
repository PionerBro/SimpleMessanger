#ifndef USERINFO_H
#define USERINFO_H

#include "imageinfo.h"

#include <string>
#include <map>

class UserInfo
{
public:
    UserInfo();

    void set_image(const ImageInfo& info);
    void set_login(const std::string& login);
    void set_password(const std::string& password);
    void set_name(const std::string& name);
    void set_surname(const std::string& surname);
    void set_rooms(const std::map<uint64_t, std::string>& rooms);
    void set_friends(const std::map<uint64_t, std::string>& friends);
    void add_room(uint64_t id, const std::string& name);
    void add_friend(uint64_t id, const std::string& name);
    void erase_friend(uint64_t id);
    void clear();

    const ImageInfo& image()const;
    const std::string& login()const;
    const std::string& password()const;
    const std::string& name()const;
    const std::string& surname()const;
    const std::map<uint64_t, std::string>& rooms()const;
    const std::map<uint64_t, std::string>& friends()const;
    bool is_logined()const;
    uint64_t size()const;
private:
    ImageInfo image_;
    std::string login_;
    std::string password_;
    std::string name_;
    std::string surname_;
    std::map<uint64_t, std::string> rooms_;
    std::map<uint64_t, std::string> friends_;
};

#endif // USERINFO_H
