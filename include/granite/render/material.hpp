#pragma once

#include "granite/render/shader.hpp"
#include "granite/core/color.hpp"
#include "granite/assets/texture.hpp"

namespace gr::Render {

class Material {
public:
    gr::Color3 color = {1.0f, 1.0f, 1.0f};
    float shininess = 32.0f;
    float opacity = 1.0f;
    gr::Render::Shader* shader = nullptr;
    gr::Assets::Texture texture;

    void bind();
private:
    GLint cL_ = -1;
    GLint sL_ = -1;
    GLint oL_ = -1;
    GLint tL_ = -1;
    GLint hL_ = -1;
};

}