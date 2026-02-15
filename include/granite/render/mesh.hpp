#pragma once

#include "glad/glad.h"
#include <vector>

#include "granite/render/shader.hpp"
#include "granite/core/color.hpp"

namespace gr::Render {

class Mesh {
public:
    Mesh();
    ~Mesh();
    void upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index, const std::vector<float>& normals);
    void draw(const Shader& shader) const;

    void newTriangle();
    void newQuad();
    void newCircle(int segments = 64);

    void newCube();
    void newSphere(int latSegments = 32, int longSegments = 64);
    void newCylinder(int segments = 64);
    void newPyramid();
    void newCone(int segments = 64);
private:
    GLuint vbo_, vao_, ebo_;
    uint32_t vertexCount_;
    uint32_t indexCount_;
};

}