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
    void upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index, const std::vector<float>& color);
    void draw(Shader shader, GLenum drawMode = GL_TRIANGLES) const;

    void newTriangle(const gr::Color3 color);
    void newQuad(const gr::Color3 color);
    void newCircle(const gr::Color3 color, int segments = 32);

    void newCube(const gr::Color3 color);
    void newSphere(const gr::Color3 color, int latSegments = 16, int longSegments = 32);
    void newCylinder(const gr::Color3 color, int segments = 32);
    void newPyramid(const gr::Color3 color);
    void newCone(const gr::Color3 color, int segments = 32);
private:
    GLuint vbo_, vao_, ebo_;
    uint32_t vertexCount_;
    uint32_t indexCount_;
    GLint cL_ = 0;
};

class RenderObject{
public:
    Mesh* mesh;
    Transform transform;
    void draw(const Shader& shader, GLenum drawMode = GL_TRIANGLES) const;
private:
    GLint mL_ = 0;
};

};