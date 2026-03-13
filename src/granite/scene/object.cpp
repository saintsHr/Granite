#include "granite/scene/object.hpp"
#include "granite/renderer/renderer.hpp"

namespace gr::Scene {

void Object::draw() {
    part_.material  = material;
    part_.transform = transform;
    part_.mesh      = &mesh;
    gr::Renderer::addToQueue(part_);
}

void ModelObject::load(gr::Assets::Model& model, std::shared_ptr<gr::Renderer::Shader> shader) {
    parts_.clear();
    parts_.reserve(model.meshes.size());

    for (size_t i = 0; i < model.meshes.size(); i++) {
        parts_.emplace_back();
        auto& obj = parts_.back();

        obj.mesh = &model.meshes[i];
        obj.material = model.materials[i];
        if (shader) obj.material.shader = shader;
        obj.transform = transform;
    }
}

void ModelObject::draw() {
    for (auto& p : parts_) {
        p.transform = transform;
        gr::Renderer::addToQueue(p);
    }
}

}