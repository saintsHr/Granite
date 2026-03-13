#pragma once

#include "granite/renderer/renderable.hpp"

namespace gr::Scene {

class Object {
public:
    gr::Transform transform;
    gr::Renderer::Material material;
    gr::Renderer::Mesh mesh;

    void draw();
private:
    gr::Scene::RenderObject part_;
};

}