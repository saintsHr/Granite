#pragma once

#include <vector>

#include "physics/body.hpp"
#include "core/vector.hpp"

namespace gr::Physics {

class World {
public:
    gr::Vec3 gravity = {0.0f, -9.8f, 0.0f};
    void step(float dt);
    void removeBody(gr::Physics::Body* body);
    void addBody(gr::Physics::Body* body);
private:
    std::vector<gr::Physics::Body*> bodies_;
};

}