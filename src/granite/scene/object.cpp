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

#include "granite/scene/object.hpp"
#include "granite/core/math.hpp"
#include "granite/renderer/renderer.hpp"

namespace gr::Scene {

void Object::draw() {
    if (model) {
        if (modelLoaded_) {
            for (auto& p : parts_) {
                p.transform = transform;
                gr::Renderer::addToQueue(p);
            }
        } else {
            parts_.clear();
            parts_.reserve(model->meshes.size());

            for (size_t i = 0; i < model->meshes.size(); i++) {
                parts_.emplace_back();
                auto& obj = parts_.back();

                obj.mesh      = &model->meshes[i];
                obj.material  = model->materials[i];
                obj.transform = transform;
            }

            modelLoaded_ = true;
        }
    } else {
        part_.material  = material;
        part_.transform = transform;
        part_.mesh      = mesh.get();
        gr::Renderer::addToQueue(part_);
    }
}

void Object::update(void) {
    if (body) {
        if (!body->isBuilt_()) body->build();
        transform = body->transform;
    }
}

void Object::reloadModel(void) {
    if (model) {
        parts_.clear();
        parts_.reserve(model->meshes.size());

        for (size_t i = 0; i < model->meshes.size(); i++) {
            parts_.emplace_back();
            auto& obj = parts_.back();

            obj.mesh      = &model->meshes[i];
            obj.material  = model->materials[i];
            obj.transform = transform;
        }

        modelLoaded_ = true;
    }
}

gr::Vec3 Object::getDirection(gr::Direction direction) {
    float rx = gr::Math::Deg2Rad(transform.rotation.x);
    float ry = gr::Math::Deg2Rad(transform.rotation.y);
    float rz = gr::Math::Deg2Rad(transform.rotation.z);

    float cx = std::cos(rx), sx = std::sin(rx);
    float cy = std::cos(ry), sy = std::sin(ry);
    float cz = std::cos(rz), sz = std::sin(rz);

    gr::Vec3 v;
    switch(direction){
        case gr::Direction::FRONT:  v = { 0.0f,  0.0f, -1.0f}; break;
        case gr::Direction::BACK:   v = { 0.0f,  0.0f,  1.0f}; break;
        case gr::Direction::RIGHT:  v = { 1.0f,  0.0f,  0.0f}; break;
        case gr::Direction::LEFT:   v = {-1.0f,  0.0f,  0.0f}; break;
        case gr::Direction::UP:     v = { 0.0f,  1.0f,  0.0f}; break;
        case gr::Direction::DOWN:   v = { 0.0f, -1.0f,  0.0f}; break;
        default: return {0,0,0};
    }

    float x = v.x*(cy*cz) + v.y*(sx*sy*cz - cx*sz) + v.z*(cx*sy*cz + sx*sz);
    float y = v.x*(cy*sz) + v.y*(sx*sy*sz + cx*cz) + v.z*(cx*sy*sz - sx*cz);
    float z = v.x*(-sy)   + v.y*(sx*cy)            + v.z*(cx*cy);

    return {x, y, z};
}

}
