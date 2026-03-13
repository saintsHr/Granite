#include "granite/scene/object.hpp"
#include "granite/renderer/renderer.hpp"

gr::Vec3 calculateDirection(gr::Vec3 rotation, gr::Direction direction) {
    float rx = gr::Math::deg2rad(rotation.x);
    float ry = gr::Math::deg2rad(rotation.y);
    float rz = gr::Math::deg2rad(rotation.z);

    float cx = std::cos(rx), sx = std::sin(rx);
    float cy = std::cos(ry), sy = std::sin(ry);
    float cz = std::cos(rz), sz = std::sin(rz);

    gr::Vec3 v;
    switch(direction){
        case gr::Direction::FRONT:  v = { 0.0f,  0.0f,  1.0f}; break;
        case gr::Direction::BACK:   v = { 0.0f,  0.0f, -1.0f}; break;
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

namespace gr::Scene {

void Object::draw() {
    part_.material  = material;
    part_.transform = transform;
    part_.mesh      = &mesh;
    gr::Renderer::addToQueue(part_);
}

gr::Vec3 Object::getDirection(gr::Direction direction) {
    return calculateDirection(transform.rotation, direction);
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

gr::Vec3 ModelObject::getDirection(gr::Direction direction) {
    return calculateDirection(transform.rotation, direction);
}

}