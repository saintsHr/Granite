#include "granite/scene/object.hpp"
#include "granite/renderer/renderer.hpp"

namespace gr::Scene {

void Object::draw() {
    part_.material  = material;
    part_.transform = transform;
    part_.mesh      = &mesh;
    gr::Renderer::addToQueue(part_);
}

};