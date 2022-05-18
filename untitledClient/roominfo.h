#ifndef ROOMINFO_H
#define ROOMINFO_H

#include <map>
#include <string>
#include <vector>

class RoomInfo
{
public:
    RoomInfo();


    void set_name(const std::string& name);
    void set_members(const std::map<uint64_t, std::string>& members);
    void set_messages(const std::vector<std::pair<std::string, std::string>>& messages);
    void add_members(uint64_t id, const std::string& name);
    void add_message(const std::string& user_name, const std::string& message);
    void clear();

    const std::string& name()const;
    const std::map<uint64_t, std::string>& members()const;
    const std::vector<std::pair<std::string, std::string>>& messages()const;
    uint64_t size()const;
private:
    std::string name_;
    std::map<uint64_t, std::string> members_;
    std::vector<std::pair<std::string, std::string>> messages_;
};

#endif // ROOMINFO_H
