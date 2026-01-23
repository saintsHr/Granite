#pragma once

#define PI 3.1415926535

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

    void newTriangle();
    void newQuad();
    void newCircle(int segments = 32);

    void newCube();
    void newSphere(int latSegments = 16, int longSegments = 32);
    void newCylinder(int segments = 32);
    void newPyramid();
    void newCone(int segments = 32);
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
    void draw(const Shader& shader, gr::Color3 color, GLenum drawMode = GL_TRIANGLES) const;
private:
    GLint mL_ = 0;
};

};