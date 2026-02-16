#include "granite/assets/texture.hpp"
#include "granite/core/log.hpp"

namespace gr::Assets {

void Texture::load(const gr::Assets::Image& image) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    if (image.data() == nullptr || image.width() == 0 || image.height() == 0) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::ASSETS,
            "Cannot load texture of id {}",
            id_
        );
        id_ = 0;
    } else {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            image.width(),
            image.height(),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.data()
        );
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    switch (filter_) {
        case TextureFilter::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmaps_ ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureFilter::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmaps_ ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        default:
            break;
    }

    if (useMipmaps_) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::setFilter(TextureFilter filter, bool useMipmaps) {
    glBindTexture(GL_TEXTURE_2D, id_);

    filter_ = filter;
    useMipmaps_ = useMipmaps;

    switch(filter) {
        case TextureFilter::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureFilter::LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        default:
            break;
    }

    if (useMipmaps) glGenerateMipmap(GL_TEXTURE_2D);
}

}