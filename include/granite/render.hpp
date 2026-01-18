#pragma once

#include "glad/glad.h"
#include "granite/vector.hpp"
#include "granite/color.hpp"
#include <vector>

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
    void upload(const std::vector<float>& vertices);
    void upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index);
    void draw(Shader shader, gr::Color3 color, GLenum drawMode = GL_TRIANGLES) const;
private:
    GLuint vbo_, vao_, ebo_;
    uint32_t vertexCount_;
    uint32_t indexCount_;
};

class RenderObject{
public:
    Mesh* mesh;
    Transform transform;
    void draw(const Shader& shader, gr::Color3 color) const;
};

};