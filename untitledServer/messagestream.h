#ifndef MESSAGESTREAM_H
#define MESSAGESTREAM_H

#include <cstdint>
#include <vector>
#include <string>
#include "userinfo.h"
#include "roominfo.h"
#include "imageinfo.h"

class MessageStream
{
    static constexpr uint64_t string_length_size = 8;
public:
    MessageStream();
    MessageStream(const char* ptr, uint64_t size);
    MessageStream(const std::string& str);
    MessageStream(const MessageStream&)= delete;
    MessageStream& operator=(const MessageStream&)= delete;

    void reserve(uint64_t size);
    void clear();

    uint64_t size()const;
    uint64_t capacity()const;
    char* data();
    const char* data()const;
    std::string& row_data();
    const std::string& row_data()const;
    uint64_t read_pos()const;
    uint64_t write_pos()const;
    bool eof()const;

    MessageStream& operator<<(const ImageInfo& info);
    MessageStream& operator<<(const RoomInfo& info);
    MessageStream& operator<<(const UserInfo& info);
    MessageStream& operator<<(const std::string& str);
    MessageStream& operator<<(const char* ptr);
    MessageStream& operator<<(int8_t i);
    MessageStream& operator<<(int16_t i);
    MessageStream& operator<<(int32_t i);
    MessageStream& operator<<(int64_t i);
    MessageStream& operator<<(uint8_t i);
    MessageStream& operator<<(uint16_t i);
    MessageStream& operator<<(uint32_t i);
    MessageStream& operator<<(uint64_t i);

    MessageStream& operator>>(ImageInfo& info);
    MessageStream& operator>>(RoomInfo& info);
    MessageStream& operator>>(UserInfo& info);
    MessageStream& operator>>(std::string& str);
    MessageStream& operator>>(char* &ptr);
    MessageStream& operator>>(int8_t& i);
    MessageStream& operator>>(int16_t& i);
    MessageStream& operator>>(int32_t& i);
    MessageStream& operator>>(int64_t& i);
    MessageStream& operator>>(uint8_t& i);
    MessageStream& operator>>(uint16_t& i);
    MessageStream& operator>>(uint32_t& i);
    MessageStream& operator>>(uint64_t& i);

    const MessageStream& operator>>(ImageInfo& info) const;
    const MessageStream& operator>>(RoomInfo& info) const;
    const MessageStream& operator>>(UserInfo& info) const;
    const MessageStream& operator>>(std::string& str) const;
    const MessageStream& operator>>(char* &ptr) const;
    const MessageStream& operator>>(int8_t& i) const;
    const MessageStream& operator>>(int16_t& i) const;
    const MessageStream& operator>>(int32_t& i) const;
    const MessageStream& operator>>(int64_t& i) const;
    const MessageStream& operator>>(uint8_t& i) const;
    const MessageStream& operator>>(uint16_t& i) const;
    const MessageStream& operator>>(uint32_t& i) const;
    const MessageStream& operator>>(uint64_t& i) const;
private:
    std::string data_;
    uint64_t write_pos_ = 0;
    mutable uint64_t read_pos_ = 0;
};

#endif // MESSAGESTREAM_H
