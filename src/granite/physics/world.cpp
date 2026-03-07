#include <algorithm>

#include "physics/world.hpp"

namespace gr::Physics {
    void World::step(float dt){

    }

    void World::removeBody(gr::Physics::Body* body){
        bodies_.erase(
            std::remove(bodies_.begin(), bodies_.end(), body),
            bodies_.end()
        );
    }

    void World::addBody(gr::Physics::Body* body){
        bodies_.push_back(body);
    }
}