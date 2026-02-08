#pragma once

#include "granite/render/shader.hpp"
#include "granite/core/color.hpp"

namespace gr::Render {

class Material {
public:
    gr::Color3 color;
    float shininess = 32.0f;
    gr::Render::Shader* shader = nullptr;
    void bind();
private:
    GLint cL_ = -1;
    GLint sL_ = -1;
};

}