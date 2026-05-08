/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <vector>

#include "granite/renderer/shader.hpp"
#include <memory>

namespace gr::Renderer {

class Mesh {
public:
    Mesh();
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index, const std::vector<float>& normals, const std::vector<float>& uvs);
    void draw(const Shader& shader) const;

    static std::shared_ptr<gr::Renderer::Mesh> newTriangle();
    static std::shared_ptr<gr::Renderer::Mesh> newQuad();
    static std::shared_ptr<gr::Renderer::Mesh> newCircle(int segments = 64);

    static std::shared_ptr<gr::Renderer::Mesh> newCube();
    static std::shared_ptr<gr::Renderer::Mesh> newSphere(int latSegments = 32, int longSegments = 64);
    static std::shared_ptr<gr::Renderer::Mesh> newCylinder(int segments = 64);
    static std::shared_ptr<gr::Renderer::Mesh> newPyramid();
    static std::shared_ptr<gr::Renderer::Mesh> newCone(int segments = 64);
private:
    unsigned int vbo_, vao_, ebo_;
    uint32_t vertexCount_;
    uint32_t indexCount_;
};

}
