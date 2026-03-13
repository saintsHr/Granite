#pragma once

#include "granite/renderer/renderable.hpp"

namespace gr::Scene {

class Object {
public:
    gr::Transform transform;
    gr::Renderer::Material material;
    gr::Renderer::Mesh mesh;

    void draw();
    gr::Vec3 getDirection(gr::Direction direction);
private:
    gr::Renderer::RenderObject part_;
};

class ModelObject {
public:
    gr::Transform transform;
    
    void load(gr::Assets::Model& model, std::shared_ptr<gr::Renderer::Shader> shader = nullptr);
    void draw();
    gr::Vec3 getDirection(gr::Direction direction);
private:
    std::vector<gr::Renderer::RenderObject> parts_;
};

}