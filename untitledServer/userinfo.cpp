#include "userinfo.h"

UserInfo::UserInfo()
{

}

void UserInfo::set_image(const ImageInfo &info)
{
    image_ = info;
}

void UserInfo::set_login(const std::string &login)
{
    login_ = login;
}

void UserInfo::set_password(const std::string& password)
{
    password_ = password;
}

void UserInfo::set_name(const std::string &name)
{
    name_ = name;
}

void UserInfo::set_surname(const std::string &surname)
{
    surname_ = surname;
}

void UserInfo::set_rooms(const std::map<uint64_t, std::string> &rooms)
{
    rooms_ = rooms;
}

void UserInfo::set_friends(const std::map<uint64_t, std::string>& friends)
{
    friends_ = friends;
}

void UserInfo::add_room(uint64_t id, const std::string &name)
{
    rooms_.emplace(id, name);
}

void UserInfo::add_friend(uint64_t id, const std::string &name)
{
    friends_.emplace(id, name);
}

void UserInfo::erase_friend(uint64_t id)
{
    friends_.erase(id);
}

void UserInfo::clear()
{
    image_.clear();
    login_.clear();
    password_.clear();
    name_.clear();
    surname_.clear();
    rooms_.clear();
    friends_.clear();
}

const ImageInfo &UserInfo::image() const
{
    return image_;
}

const std::string &UserInfo::login() const
{
    return login_;
}

const std::string &UserInfo::password() const
{
    return password_;
}

const std::string &UserInfo::name() const
{
    return name_;
}

const std::string &UserInfo::surname() const
{
    return surname_;
}

const std::map<uint64_t, std::string> &UserInfo::rooms() const
{
    return rooms_;
}

const std::map<uint64_t, std::string> &UserInfo::friends() const
{
    return friends_;
}

bool UserInfo::is_logined() const
{
    return !login_.empty();
}

uint64_t UserInfo::size() const
{
    uint64_t size = login_.size()+password_.size()+name_.size()+surname_.size();
    for(const auto& room:rooms_)
        size += 8+room.second.size();
    for(const auto& fr:friends_)
        size += 8+fr.second.size();
    return size+image_.size()+28;
}
