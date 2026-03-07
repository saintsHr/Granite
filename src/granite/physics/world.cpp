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
        for (int i = 0; i < bodies_.size(); i++) {
            world_->removeRigidBody(bodies_.at(i)->getRaw_());
        }

        bodies_.clear();

        delete world_;
        delete solver_;
        delete broadphase_;
        delete dispatcher_;
        delete collisionConfig_;
    }

    void World::step(float dt){
        world_->stepSimulation(dt, 10, 1.0f / 60.0f);

        for (int i = 0; i < bodies_.size(); i++) {
            bodies_.at(i)->sync();
        }
    }

    void World::removeBody(gr::Physics::Body* body){
        bodies_.erase(
            std::remove(bodies_.begin(), bodies_.end(), body),
            bodies_.end()
        );
        world_->removeRigidBody(body->getRaw_());
    }

    void World::addBody(gr::Physics::Body* body){
        bodies_.push_back(body);
        world_->addRigidBody(body->getRaw_());
    }
}