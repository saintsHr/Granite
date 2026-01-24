#pragma once

#include "granite/core/vector.hpp"
#include "granite/render/mesh.hpp"

namespace gr::Scene{

class RenderObject{
public:
    gr::Render::Mesh* mesh;
    Transform transform;
    void draw(const gr::Render::Shader& shader, GLenum drawMode = GL_TRIANGLES) const;
private:
    GLint mL_ = 0;
};

}