#pragma once

#include "granite/core/vector.hpp"
#include "granite/render/mesh.hpp"
#include "granite/render/material.hpp"

namespace gr::Scene{

class RenderObject{
public:
    gr::Render::Mesh* mesh;
    Transform transform;
    gr::Render::Material material;
    void draw();
private:
    GLint mL_ = -1;
    GLint vL_ = -1;
    GLint pL_ = -1;
};

}