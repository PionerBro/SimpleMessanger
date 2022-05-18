#include "imageinfo.h"

#include <cstring>

ImageInfo::ImageInfo():data_(nullptr), size_(0), width_(0),height_(0),format_(0)
{

}

ImageInfo::ImageInfo(const unsigned char *data, uint64_t size, uint64_t width, uint64_t height, uint32_t format):data_(nullptr), size_(0)
{
    if(size){
        data_ = new unsigned char[size];
        memcpy(data_, data, size);
        size_ = size;
    }
    width_ = width;
    height_ = height;
    format_ = format;
}

ImageInfo::ImageInfo(const ImageInfo &other):data_(nullptr), size_(0)
{
    if(other.size_){
        data_ = new unsigned char[other.size_];
        memcpy(data_, other.data_, other.size_);
        size_ = other.size_;
    }
    width_ = other.width_;
    height_ = other.height_;
    format_ = other.format_;
}

ImageInfo &ImageInfo::operator=(const ImageInfo &other)
{
    if(this == &other)
        return *this;
    if(other.size_){
        unsigned char* tmp = new unsigned char[other.size_];
        memcpy(tmp, other.data_, other.size_);
        if(nullptr != data_)
            delete[] data_;
        data_ = tmp;
        size_ = other.size_;
    }else{
        if(nullptr != data_)
            delete[] data_;
        data_ = nullptr;
        size_ = 0;
    }
    width_ = other.width_;
    height_ = other.height_;
    format_ = other.format_;
    return *this;
}

ImageInfo::~ImageInfo()
{
    if(data_)
        delete[] data_;
}

void ImageInfo::set_data(const unsigned char *data, uint64_t size)
{
    if(size){
        unsigned char* tmp_ptr = new unsigned char[size];
        memcpy(tmp_ptr, data, size);
        if(nullptr != data_)
            delete []data_;
        data_ = tmp_ptr;
        size_ = size;
    }else{
        if(nullptr != data_)
            delete []data_;
        data_ = nullptr;
        size_ = 0;
    }
}

void ImageInfo::set_width(uint64_t width)
{
    width_ = width;
}

void ImageInfo::set_height(uint64_t height)
{
    height_ = height;
}

void ImageInfo::set_format(uint32_t format)
{
    format_ = format;
}

void ImageInfo::clear()
{
    if(data_)
        delete []data_;
    data_ = nullptr;
    size_ = 0;
    width_ = 0;
    height_ = 0;
    format_ = 0;
}

const unsigned char *ImageInfo::data() const
{
    return data_;
}

uint64_t ImageInfo::size() const
{
    return size_;
}

uint64_t ImageInfo::width() const
{
    return width_;
}

uint64_t ImageInfo::height() const
{
    return height_;
}

uint32_t ImageInfo::format() const
{
    return format_;
}

bool ImageInfo::isEmpty() const
{
    return !(data_ && size_ && width_ && height_ && format_);
}
