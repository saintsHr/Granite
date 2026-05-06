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

#include <algorithm>
#include <btBulletDynamicsCommon.h>

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

    World::~World(void) {
        for (long unsigned int i = 0; i < bodies_.size(); i++) {
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
        world_->stepSimulation(std::min(dt, 0.1f), 8, 1.0f / 60.0f);

        for (long unsigned int i = 0; i < bodies_.size(); i++) {
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
