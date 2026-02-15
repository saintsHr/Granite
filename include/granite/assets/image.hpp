#pragma once

#include <string>

#include "stb_image/stb_image.h"
#include "granite/core/vector.hpp"

namespace gr::Assets {

class Image {
public:
    Image() = default;
    ~Image();

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& other) noexcept;
    Image& operator=(Image&& other) noexcept;

    void load(const std::string& filename);

    int width() const { return static_cast<int>(size_.x); }
    int height() const { return static_cast<int>(size_.y); }
    bool isValid() const { return data_ != nullptr; }
    unsigned char* data() { return data_; }
    const unsigned char* data() const { return data_; }
private:
    unsigned char* data_ = nullptr;
    gr::Vec2 size_ = {0.0f, 0.0f};
};

}