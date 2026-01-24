#pragma once

#include "glad/glad.h"
#include <vector>

#include "granite/render/shader.hpp"
#include "granite/core/color.hpp"

namespace gr::Render{

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

}