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

#pragma once

#include "granite/core/vector.hpp"

#include <vector>

class btRigidBody;
class btCollisionShape;
class btMotionState;
class btDynamicsWorld;

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

class PhysicsMaterial {
public:
    float bounciness       = 0.00f;
    float friction         = 0.50f;
    float mass             = 1.00f;
    float linearDamping    = 0.01f;
    float angularDamping   = 0.05f;
    float rollingFriction  = 0.00f;
    float spinningFriction = 0.02f;
};

class Body {
public:
    ~Body();

    gr::Transform transform;

    PhysicsMaterial material;

    BodyType type = BodyType::DYNAMIC;
    Shape shape = Shape::CUBE;

    void build();
    void sync();

    void applyCentralForce(gr::Vec3 force);
    void applyForce(gr::Vec3 force, gr::Vec3 pos);

    void applyCentralImpulse(gr::Vec3 impulse);
    void applyImpulse(gr::Vec3 impulse, gr::Vec3 pos);

    void applyTorqueImpulse(gr::Vec3 torque);
    void applyTorque(gr::Vec3 torque);

    void setLinearVelocity(gr::Vec3 velocity);
    void setAngularVelocity(gr::Vec3 velocity);

    gr::Vec3 getLinearVelocity();
    gr::Vec3 getAngularVelocity();

    std::vector<btRigidBody*> getCollisions(btDynamicsWorld* world);

    btRigidBody* getRaw_();
    bool isBuilt_();
private:
    btRigidBody*      body_   = nullptr;
    btCollisionShape* shape_  = nullptr;
    btMotionState*    motion_ = nullptr;

    bool built_ = false;
};

}
