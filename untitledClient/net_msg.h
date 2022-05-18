#ifndef NET_MSG_H
#define NET_MSG_H

#include <cstdint>


class net_msg
{
public:
    static constexpr uint64_t header_size_ = 8;
public:
    net_msg();
    net_msg(const char* data, uint64_t size);
    net_msg(const net_msg& other);
    net_msg& operator=(const net_msg& other);
    ~net_msg();

    uint64_t header()const;
    uint64_t body_size()const;
    uint64_t size()const;
    char* body_data();
    const char* body_data()const;
    char* data();
    const char* data()const;
    void allocate_size_for_body();
private:
    bool check_byte_order();
private:
    char* ptr_ = nullptr;
    uint64_t size_ = 0;
    bool byte_order_ = false;
};

#endif // NET_MSG_H
