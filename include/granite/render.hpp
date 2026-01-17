#pragma once

#include "glad/glad.h"
#include "granite/vector.hpp"

namespace gr::Render{
    
void init();

struct Transform{
    gr::Vec3 position;
    gr::Vec3 rotation;
    gr::Vec3 scale;
};

class Mesh{
public:
    Mesh();
    ~Mesh();
    void upload(const float* vertices, uint32_t vertexCount);
    void draw(GLenum drawMode = GL_TRIANGLES) const;
private:
    GLuint vbo_, vao_;
    uint32_t vertexCount_;
};

class RenderObject{
public:
    Mesh* mesh;
    Transform transform;
};

};