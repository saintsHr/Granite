#pragma once

#include "glad/glad.h"
#include "granite/vector.hpp"

namespace gr::Render{
    
void init();

struct Transform{
    gr::Vec3 position = {0.0f, 0.0f, 0.0f};
    gr::Vec3 rotation = {0.0f, 0.0f, 0.0f};
    gr::Vec3 scale    = {1.0f, 1.0f, 1.0f};
};

class Shader{
public:
    Shader();
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();
    void use() const;
    GLuint getProgram() const;
private:
    GLuint program_;
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
    void draw(const Shader& shader) const;
};

};