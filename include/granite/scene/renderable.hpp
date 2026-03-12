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

#include "granite/core/vector.hpp"
#include "granite/render/mesh.hpp"
#include "granite/render/material.hpp"
#include "granite/assets/model.hpp"

namespace gr::Assets {
class Model;
}

namespace gr::Scene {

class RenderObject {
public:
    gr::Render::Mesh* mesh;
    Transform transform;
    gr::Render::Material material;
    void draw();
private:
    GLint mL_ = -1;
    GLint vL_ = -1;
    GLint pL_ = -1;
};

class ModelObject {
public:
    Transform transform;
    std::vector<RenderObject> parts;

    void upload(gr::Assets::Model& model);
    void upload(gr::Assets::Model& model, std::shared_ptr<gr::Render::Shader> shader);
    void draw();
};

}