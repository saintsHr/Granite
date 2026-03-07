#pragma once

#include <vector>
#include <bullet/btBulletDynamicsCommon.h>

#include "granite/physics/body.hpp"
#include "granite/core/vector.hpp"

namespace gr::Physics {

class World {
public:
    World(gr::Vec3 gravity);
    ~World();

    void step(float dt);
    void removeBody(gr::Physics::Body* body);
    void addBody(gr::Physics::Body* body);
private:
    std::vector<gr::Physics::Body*> bodies_;

    btDefaultCollisionConfiguration*     collisionConfig_ = nullptr;
    btCollisionDispatcher*               dispatcher_      = nullptr;
    btBroadphaseInterface*               broadphase_      = nullptr;
    btSequentialImpulseConstraintSolver* solver_          = nullptr;
    btDiscreteDynamicsWorld*             world_           = nullptr;
};

}