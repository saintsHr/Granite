#pragma once

#include "granite/core/vector.hpp"

#include <bullet/btBulletDynamicsCommon.h>

namespace gr::Physics {

enum class BodyType {
    STATIC,
    DYNAMIC,
    KINEMATIC
};

enum class Shape {
    CUBE,
    SPHERE,
    CAPSULE,
    CONE,
    CYLINDER
};

class Body {
public:
    ~Body();

    gr::Transform transform;
    float mass = 1.0f;

    BodyType type = BodyType::DYNAMIC;
    Shape shape = Shape::CUBE;

    void build();
    void sync();
    btRigidBody* getRaw_();
private:
    btRigidBody*      body_   = nullptr;
    btCollisionShape* shape_  = nullptr;
    btMotionState*    motion_ = nullptr;
};

}