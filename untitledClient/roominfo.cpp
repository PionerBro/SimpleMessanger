#include "roominfo.h"

RoomInfo::RoomInfo()
{

}

void RoomInfo::set_name(const std::string &name)
{
    name_ = name;
}

void RoomInfo::set_members(const std::map<uint64_t, std::string> &members)
{
    members_ = members;
}

void RoomInfo::set_messages(const std::vector<std::pair<std::string, std::string> > &messages)
{
    messages_ = messages;
}

void RoomInfo::add_members(uint64_t id, const std::string &name)
{
    members_.emplace(id, name);
}

void RoomInfo::add_message(const std::string &user_name, const std::string &message)
{
    messages_.emplace_back(user_name, message);
}

void RoomInfo::clear()
{
    name_.clear();
    members_.clear();
    messages_.clear();
}

const std::string &RoomInfo::name() const
{
    return name_;
}

const std::map<uint64_t, std::string> &RoomInfo::members() const
{
    return members_;
}

const std::vector<std::pair<std::string, std::string> > &RoomInfo::messages() const
{
    return messages_;
}

uint64_t RoomInfo::size() const
{
    uint64_t size = name_.size();
    for(const auto& member:members_)
        size += 8 + member.second.size();
    for(const auto& message:messages_)
        size += message.first.size()+message.second.size();
    return size;
}
