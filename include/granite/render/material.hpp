#pragma once

#include "granite/render/shader.hpp"
#include "granite/core/color.hpp"

namespace gr::Render {

class Material {
public:
    gr::Color3 color;
    gr::Render::Shader* shader = nullptr;
    void bind();
private:
    GLint cL_ = -1;
};

}