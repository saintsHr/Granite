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
            filename
        );

        size_.x = 0.0f;
        size_.y = 0.0f;

        return;
    }

    size_.x = static_cast<float>(width);
    size_.y = static_cast<float>(height);
}

}