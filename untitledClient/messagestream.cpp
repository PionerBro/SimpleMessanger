#include "messagestream.h"

MessageStream::MessageStream():write_pos_(0), read_pos_(0)
{

}

MessageStream::MessageStream(const char *ptr, uint64_t size): data_(ptr, size), write_pos_(size), read_pos_(0)
{

}

MessageStream::MessageStream(const std::string &str):data_(str), write_pos_(str.size()), read_pos_(0)
{

}

void MessageStream::reserve(uint64_t size)
{
    data_.reserve(size);
}

void MessageStream::clear()
{
    data_.clear();
    write_pos_ = 0;
    read_pos_ = 0;
}

uint64_t MessageStream::size() const
{
    return data_.size();
}

uint64_t MessageStream::capacity() const
{
    return data_.capacity();
}

char *MessageStream::data()
{
    return data_.data();
}

const char *MessageStream::data() const
{
    return data_.data();
}

std::string &MessageStream::row_data()
{
    return data_;
}

const std::string &MessageStream::row_data() const
{
    return data_;
}

uint64_t MessageStream::read_pos() const
{
    return read_pos_;
}

uint64_t MessageStream::write_pos() const
{
    return write_pos_;
}

bool MessageStream::eof() const
{
    return read_pos_ == data_.size();
}

MessageStream& MessageStream::operator<<(const ImageInfo& info){
    uint64_t block_size = info.size()+28;
    if(write_pos_+block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    *this<<std::string((const char*)info.data(), info.size())<<info.width()<<info.height()<<info.format();
    return *this;
}

MessageStream& MessageStream::operator>>(ImageInfo& info){
    info.clear();
    std::string data;
    uint64_t width = 0;
    uint64_t height = 0;
    uint32_t format = 0;
    *this>>data>>width>>height>>format;
    info.set_data((const unsigned char*)data.data(), data.size());
    info.set_width(width);
    info.set_height(height);
    info.set_format(format);
    return *this;
}

const MessageStream& MessageStream::operator>>(ImageInfo& info) const{
    info.clear();
    std::string data;
    uint64_t width = 0;
    uint64_t height = 0;
    uint32_t format = 0;
    *this>>data>>width>>height>>format;
    info.set_data((const unsigned char*)data.data(), data.size());
    info.set_width(width);
    info.set_height(height);
    info.set_format(format);
    return *this;
}

MessageStream& MessageStream::operator<<(const RoomInfo& info){
    uint64_t block_size = info.size() + 8 + 8*info.members().size() + 16*info.messages().size() + 16;//
    if(write_pos_+block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    *this<<info.name()<<info.members().size();
    for(const auto& member : info.members())
        *this<<member.first<<member.second;
    *this<<info.messages().size();
    for(const auto& message:info.messages())
        *this<<message.first<<message.second;
    return *this;
}

MessageStream& MessageStream::operator>>(RoomInfo& info){
    info.clear();
    std::string name;
    *this>>name;
    info.set_name(name);
    uint64_t members_size = 0;
    *this>>members_size;
    for(uint64_t i = 0; i < members_size; ++i){
        uint64_t member_id = 0;
        std::string member_name;
        *this>>member_id>>member_name;
        info.add_members(member_id, member_name);
    }
    uint64_t messages_size = 0;
    *this>>messages_size;
    for(uint64_t i = 0; i < messages_size; ++i){
        std::string sender_name;
        std::string message;
        *this>>sender_name>>message;
        info.add_message(sender_name, message);
    }
    return *this;
}

const MessageStream& MessageStream::operator>>(RoomInfo& info) const{
    info.clear();
    std::string name;
    *this>>name;
    info.set_name(name);
    uint64_t members_size = 0;
    *this>>members_size;
    for(uint64_t i = 0; i < members_size; ++i){
        uint64_t member_id = 0;
        std::string member_name;
        *this>>member_id>>member_name;
        info.add_members(member_id, member_name);
    }
    uint64_t messages_size = 0;
    *this>>messages_size;
    for(uint64_t i = 0; i < messages_size; ++i){
        std::string sender_name;
        std::string message;
        *this>>sender_name>>message;
        info.add_message(sender_name, message);
    }
    return *this;
}

MessageStream& MessageStream::operator<<(const UserInfo& info){
    uint64_t block_size = info.size() + 32 + 8*(info.friends().size()+info.rooms().size()) +16;//
    if(write_pos_+block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    *this<<info.image()<<info.login()<<info.password()<<info.name()<<info.surname()<<info.rooms().size();
    for(const auto& room:info.rooms())
        *this<<room.first<<room.second;
    *this<<info.friends().size();
    for(const auto& fr:info.friends())
        *this<<fr.first<<fr.second;
    return *this;
}

MessageStream& MessageStream::operator>>(UserInfo& info){
    info.clear();
    ImageInfo image;
    std::string login;
    std::string password;
    std::string name;
    std::string surname;
    *this>>image>>login>>password>>name>>surname;
    info.set_image(image);
    info.set_login(login);
    info.set_password(password);
    info.set_name(name);
    info.set_surname(surname);
    uint64_t rooms_size = 0;
    *this>>rooms_size;
    for(uint64_t i = 0; i < rooms_size; ++i){
        uint64_t id = 0;
        std::string room_name;
        *this>>id>>room_name;
        info.add_room(id, room_name);
    }
    uint64_t friends_size = 0;
    *this>>friends_size;
    for(uint64_t i = 0; i < friends_size; ++i){
        uint64_t id = 0;
        std::string friend_name;
        *this>>id>>friend_name;
        info.add_friend(id, friend_name);
    }
    return *this;
}

const MessageStream& MessageStream::operator>>(UserInfo& info) const{
    info.clear();
    ImageInfo image;
    std::string login;
    std::string password;
    std::string name;
    std::string surname;
    *this>>image>>login>>password>>name>>surname;
    info.set_image(image);
    info.set_login(login);
    info.set_password(password);
    info.set_name(name);
    info.set_surname(surname);
    uint64_t rooms_size = 0;
    *this>>rooms_size;
    for(uint64_t i = 0; i < rooms_size; ++i){
        uint64_t id = 0;
        std::string room_name;
        *this>>id>>room_name;
        info.add_room(id, room_name);
    }
    uint64_t friends_size = 0;
    *this>>friends_size;
    for(uint64_t i = 0; i < friends_size; ++i){
        uint64_t id = 0;
        std::string friend_name;
        *this>>id>>friend_name;
        info.add_friend(id, friend_name);
    }
    return *this;
}

MessageStream &MessageStream::operator<<(const std::string &str)
{
    uint64_t block_size = str.size();
    if(write_pos_+sizeof(uint64_t)+block_size > data_.size()){
        data_.resize(write_pos_+sizeof(uint64_t)+block_size);
    }
    *this<<block_size;
    memcpy(&data_[write_pos_], str.data(), block_size);
    write_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator<<(const char* ptr){
    uint64_t block_size = std::strlen(ptr);
    if(write_pos_+sizeof(uint64_t)+block_size > data_.size()){
        data_.resize(write_pos_+sizeof(uint64_t)+block_size);
    }
    *this<<block_size;
    memcpy(&data_[write_pos_], ptr, block_size);
    write_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator<<(int8_t i){
    uint64_t block_size = sizeof(uint8_t);
    if(write_pos_ + block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    memcpy(&data_[write_pos_], (char*)&i, block_size);
    write_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator<<(int16_t i){
    uint64_t block_size = sizeof(uint16_t);
    if(write_pos_ + block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    memcpy(&data_[write_pos_], (char*)&i, block_size);
    write_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator<<(int32_t i){
    uint64_t block_size = sizeof(uint32_t);
    if(write_pos_ + block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    memcpy(&data_[write_pos_], (char*)&i, block_size);
    write_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator<<(int64_t i){
    uint64_t block_size = sizeof(uint64_t);
    if(write_pos_ + block_size > data_.size()){
        data_.resize(write_pos_+block_size);
    }
    memcpy(&data_[write_pos_], (char*)&i, block_size);
    write_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator<<(uint8_t i){
    return *this<<static_cast<int8_t>(i);
}

MessageStream& MessageStream::operator<<(uint16_t i){
    return *this<<static_cast<int16_t>(i);
}

MessageStream& MessageStream::operator<<(uint32_t i){
    return *this<<static_cast<int32_t>(i);
}

MessageStream& MessageStream::operator<<(uint64_t i){
    return *this<<static_cast<int64_t>(i);
}

MessageStream &MessageStream::operator>>(std::string &str)
{
    uint64_t str_size = 0;
    *this>>str_size;
    str.resize(str_size);
    memcpy(str.data(), &data_[read_pos_], str_size);
    read_pos_+=str_size;
    return *this;
}

MessageStream& MessageStream::operator>>(char* &ptr){
    ptr = nullptr;
    uint64_t str_size = 0;
    *this>>str_size;
    ptr = new char[str_size];
    memcpy(ptr, &data_[read_pos_], str_size);
    read_pos_+=str_size;
    return *this;
}

MessageStream& MessageStream::operator>>(int8_t& i){
    uint64_t block_size = sizeof(uint8_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}

MessageStream& MessageStream::operator>>(int16_t& i){
    uint64_t block_size = sizeof(uint16_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}
MessageStream& MessageStream::operator>>(int32_t& i){
    uint64_t block_size = sizeof(uint32_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}
MessageStream& MessageStream::operator>>(int64_t& i){
    uint64_t block_size = sizeof(uint64_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}
MessageStream& MessageStream::operator>>(uint8_t& i){
    return *this>>reinterpret_cast<int8_t&>(i);
}
MessageStream& MessageStream::operator>>(uint16_t& i){
    return *this>>reinterpret_cast<int16_t&>(i);
}
MessageStream& MessageStream::operator>>(uint32_t& i){
    return *this>>reinterpret_cast<int32_t&>(i);
}
MessageStream& MessageStream::operator>>(uint64_t& i){
    return *this>>reinterpret_cast<int64_t&>(i);
}

const MessageStream &MessageStream::operator>>(std::string &str) const
{
    uint64_t str_size = 0;
    *this>>str_size;
    str.resize(str_size);
    memcpy(str.data(), &data_[read_pos_], str_size);
    read_pos_+=str_size;
    return *this;
}

const MessageStream& MessageStream::operator>>(char* &ptr)const{
    ptr = nullptr;
    uint64_t str_size = 0;
    *this>>str_size;
    ptr = new char[str_size];
    memcpy(ptr, &data_[read_pos_], str_size);
    read_pos_+=str_size;
    return *this;
}

const MessageStream& MessageStream::operator>>(int8_t& i)const{
    uint64_t block_size = sizeof(uint8_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}

const MessageStream& MessageStream::operator>>(int16_t& i)const{
    uint64_t block_size = sizeof(uint16_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}
const MessageStream& MessageStream::operator>>(int32_t& i)const{
    uint64_t block_size = sizeof(uint32_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}
const MessageStream& MessageStream::operator>>(int64_t& i)const{
    uint64_t block_size = sizeof(uint64_t);
    memcpy((char*)&i, &data_[read_pos_], block_size);
    read_pos_+=block_size;
    return *this;
}
const MessageStream& MessageStream::operator>>(uint8_t& i)const{
    return *this>>reinterpret_cast<int8_t&>(i);
}
const MessageStream& MessageStream::operator>>(uint16_t& i)const{
    return *this>>reinterpret_cast<int16_t&>(i);
}
const MessageStream& MessageStream::operator>>(uint32_t& i)const{
    return *this>>reinterpret_cast<int32_t&>(i);
}
const MessageStream& MessageStream::operator>>(uint64_t& i)const{
    return *this>>reinterpret_cast<int64_t&>(i);
}
