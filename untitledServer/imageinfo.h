#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <cstdint>

class ImageInfo
{
public:
    enum Format{
        Format_Invalid,
        QFormat_Mono,
        QFormat_MonoLSB,
        QFormat_Indexed8,
        QFormat_RGB32,
        QFormat_ARGB32,
        QFormat_ARGB32_Premultiplied,
        QFormat_RGB16,
        QFormat_ARGB8565_Premultiplied,
        QFormat_RGB666,
        QFormat_ARGB6666_Premultiplied,
        QFormat_RGB555,
        QFormat_ARGB8555_Premultiplied,
        QFormat_RGB888,
        QFormat_RGB444,
        QFormat_ARGB4444_Premultiplied,
        QFormat_RGBX8888,
        QFormat_RGBA8888,
        QFormat_RGBA8888_Premultiplied,
        QFormat_BGR30,
        QFormat_A2BGR30_Premultiplied,
        QFormat_RGB30,
        QFormat_A2RGB30_Premultiplied,
        QFormat_Alpha8,
        QFormat_Grayscale8,
        QFormat_RGBX64,
        QFormat_RGBA64,
        QFormat_RGBA64_Premultiplied,
        QFormat_Grayscale16,
        QFormat_BGR888,
        QFormat_RGBX16FPx4,
        QFormat_RGBA16FPx4,
        QFormat_RGBA16FPx4_Premultiplied,
        QFormat_RGBX32FPx4,
        QFormat_RGBA32FPx4,
        QFormat_RGBA32FPx4_Premultiplied
    };

    ImageInfo();
    ImageInfo(const unsigned char* data, uint64_t size, uint64_t width,
                                        uint64_t height, uint32_t format);
    ImageInfo(const ImageInfo& other);
    ImageInfo& operator=(const ImageInfo& other);
    ~ImageInfo();

    void set_data(const unsigned char* data, uint64_t size);
    void set_width(uint64_t width);
    void set_height(uint64_t height);
    void set_format(uint32_t format);
    void clear();

    const unsigned char* data()const;
    uint64_t size()const;
    uint64_t width()const;
    uint64_t height()const;
    uint32_t format()const;
    bool isEmpty()const;
private:
    unsigned char* data_ = nullptr;
    uint64_t size_ = 0;
    uint64_t width_ = 0;
    uint64_t height_ = 0;
    uint32_t format_ = 0;
};

#endif // IMAGEINFO_H
