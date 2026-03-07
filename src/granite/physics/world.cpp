#include <algorithm>

#include "granite/physics/world.hpp"

namespace gr::Physics {
    World::World(gr::Vec3 gravity) {
        collisionConfig_ = new btDefaultCollisionConfiguration();
        dispatcher_      = new btCollisionDispatcher(collisionConfig_);
        broadphase_      = new btDbvtBroadphase();
        solver_          = new btSequentialImpulseConstraintSolver();

        world_ = new btDiscreteDynamicsWorld(
            reinterpret_cast<btDispatcher*>(dispatcher_),
            broadphase_,
            reinterpret_cast<btConstraintSolver*>(solver_),
            collisionConfig_
        );

        world_->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }

    World::~World() {
        delete world_;
        delete solver_;
        delete broadphase_;
        delete dispatcher_;
        delete collisionConfig_;
    }

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