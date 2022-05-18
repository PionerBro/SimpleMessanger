#include "net_msg.h"

#include <cstring>

net_msg::net_msg():ptr_(new char[header_size_]), size_(header_size_), byte_order_(check_byte_order())
{
}

net_msg::net_msg(const char *data, uint64_t size):ptr_(new char[header_size_+size]),
    size_(header_size_+size), byte_order_(check_byte_order())
{
    if(byte_order_){
        char* tmp_ptr = ptr_;
        for(uint64_t i = header_size_ - 1; i < header_size_; --i, ++tmp_ptr){
            memcpy(tmp_ptr, (char*)&size + i, sizeof(char));
        }
    }else{
        memcpy(ptr_, (char*)&size, header_size_);
    }
    memcpy(ptr_+header_size_, data, size);
}

net_msg::net_msg(const net_msg &other) : ptr_(new char[other.size_]), size_(other.size_), byte_order_(other.byte_order_)
{
    memcpy(ptr_, other.ptr_, other.size_);
}

net_msg &net_msg::operator=(const net_msg &other)
{
    if(this == &other)
        return *this;
    if(size_ != other.size_){
        delete[] ptr_;
        ptr_ = new char[other.size_];
        size_ = other.size_;
    }
    memcpy(ptr_, other.ptr_, other.size_);
    return *this;
}

net_msg::~net_msg(){   
    delete[] ptr_;
}

uint64_t net_msg::header() const
{
    uint64_t header;
    if(byte_order_){
        char* tmp_ptr = ptr_;
        for(uint64_t i = header_size_ - 1; i < header_size_; --i, ++tmp_ptr){
            memcpy((char*)&header + i, tmp_ptr, sizeof(char));
        }
    }
    else{
        memcpy((char*)&header, ptr_,header_size_);
    }
    return header;
}

uint64_t net_msg::body_size() const
{
    return size_ - header_size_;
}

uint64_t net_msg::size() const
{
    return size_;
}

char *net_msg::body_data()
{
    return ptr_+ header_size_;
}

const char *net_msg::body_data() const
{
    return ptr_+ header_size_;
}

char *net_msg::data()
{
    return ptr_;
}

const char *net_msg::data() const
{
    return ptr_;
}

void net_msg::allocate_size_for_body()
{
    size_ = header()+header_size_;
    char* new_ptr = new char[size_];
    memcpy(new_ptr, ptr_, header_size_);
    delete[] ptr_;
    ptr_ = new_ptr;
}

bool net_msg::check_byte_order()
{
    int a = 0xA1B2C3D4;
    unsigned char*p = (unsigned char*)&a;
    if(*p == 0xA1)
        return false;
    else
        return true;
}
