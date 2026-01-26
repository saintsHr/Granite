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
    void draw(GLenum drawMode = GL_TRIANGLES);
private:
    GLint mL_ = -1;
};

}