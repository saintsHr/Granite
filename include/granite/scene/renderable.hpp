#pragma once

#include "granite/core/vector.hpp"
#include "granite/render/mesh.hpp"

namespace gr::Scene{

struct Material{
    gr::Color3 color = {255, 255, 255};
};

class RenderObject{
public:
    gr::Render::Mesh* mesh;
    Transform transform;
    Material material;
    void draw(const gr::Render::Shader& shader, GLenum drawMode = GL_TRIANGLES) const;
private:
    GLint mL_ = -1;
    GLint cL_ = -1;
};

}