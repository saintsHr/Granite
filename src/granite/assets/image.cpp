/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "granite/assets/image.hpp"
#include "granite/core/log.hpp"

namespace gr::Assets {

Image::~Image() {
    stbi_image_free(data_);
    data_ = nullptr;
}

Image::Image(Image&& other) noexcept : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = {0.0f, 0.0f};
}

Image& Image::operator=(Image&& other) noexcept {
    if (this != &other) {
        if (data_)
            stbi_image_free(data_);

        data_ = other.data_;
        size_ = other.size_;

        other.data_ = nullptr;
        other.size_ = {0.0f, 0.0f};
    }
    return *this;
}

void Image::load(const std::string& filename) {
    int width = 0;
    int height = 0;

    if (data_) {
        stbi_image_free(data_);
        data_ = nullptr;
    }

    data_ = stbi_load(filename.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
    if (!data_) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::ASSETS,
            "Cannot load image %s",
            filename.c_str()
        );

        size_.x = 0.0f;
        size_.y = 0.0f;

        return;
    }

    size_.x = static_cast<float>(width);
    size_.y = static_cast<float>(height);
}

}