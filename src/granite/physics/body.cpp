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

#include "granite/physics/body.hpp"

namespace gr::Physics {

void Body::build() {
    btTransform btTransform;
    btQuaternion rot;
    btTransform.setIdentity();

    btTransform.setOrigin(btVector3(
        static_cast<btScalar>(transform.position.x),
        static_cast<btScalar>(transform.position.y),
        static_cast<btScalar>(transform.position.z)
    ));

    rot.setEulerZYX(
        transform.rotation.z * gr::Math::PI / 180.0f,
        transform.rotation.y * gr::Math::PI / 180.0f,
        transform.rotation.x * gr::Math::PI / 180.0f
    );

    btTransform.setRotation(rot);

    motion_ = new btDefaultMotionState(btTransform);

    switch (shape) {
    case Shape::CUBE:
        shape_ = new btBoxShape(btVector3(
            transform.scale.x/2,
            transform.scale.y/2,
            transform.scale.z/2
        ));
        break;

    case Shape::SPHERE:
        shape_ = new btSphereShape(
            (
                transform.scale.x +
                transform.scale.y +
                transform.scale.z
            ) / 6
        );
        break;

    case Shape::CYLINDER:
        shape_ = new btCylinderShape(btVector3(
            transform.scale.x/2,
            transform.scale.y/2,
            transform.scale.z/2
        ));
        break;

    case Shape::CAPSULE :
        shape_ = new btCapsuleShape(
            (
                transform.scale.x +
                transform.scale.y +
                transform.scale.z
            ) / 6,

            transform.scale.y
        );
        break;

    case Shape::CONE :
        shape_ = new btConeShape(
            (
                transform.scale.x +
                transform.scale.y +
                transform.scale.z
            ) / 6,

            transform.scale.y
        );
        break;
    
    default:
        break;
    }

    float bodyMass = (type == BodyType::DYNAMIC) ? mass : 0.0f;

    btVector3 inertia(0,0,0);
    if (bodyMass != 0) shape_->calculateLocalInertia(bodyMass, inertia);

    btRigidBody::btRigidBodyConstructionInfo info(
        bodyMass,
        motion_,
        shape_,
        inertia
    );

    body_ = new btRigidBody(info);

    if (type == BodyType::KINEMATIC) {
        body_->setCollisionFlags(
            body_->getCollisionFlags() |
            btCollisionObject::CF_KINEMATIC_OBJECT
        );

        body_->setActivationState(DISABLE_DEACTIVATION);
    }

    body_->setActivationState(DISABLE_DEACTIVATION);
    body_->setSleepingThresholds(0,0);
    body_->setDamping(0.1f, 0.1f);
    body_->setFriction(1.0f);
    body_->setRollingFriction(0.2f);
    body_->setSpinningFriction(0.2f);
}

void Body::sync() {
    btTransform t;
    body_->getMotionState()->getWorldTransform(t);

    btVector3 p = t.getOrigin();
    btQuaternion r = t.getRotation();
    btScalar yaw, pitch, roll;
    btMatrix3x3(r).getEulerYPR(yaw, pitch, roll);

    transform.position = {
        static_cast<float>(p.x()),
        static_cast<float>(p.y()),
        static_cast<float>(p.z())
    };

    transform.rotation = {
        roll  * 180.0f / gr::Math::PI,
        pitch * 180.0f / gr::Math::PI,
        yaw   * 180.0f / gr::Math::PI
    };
}

void Body::applyCentralForce(gr::Vec3 force) {
    btVector3 f(
        force.x,
        force.y,
        force.z
    );

    body_->applyCentralForce(f);
}

void Body::applyForce(gr::Vec3 force, gr::Vec3 pos) {
    btVector3 f(
        force.x,
        force.y,
        force.z
    );

    btVector3 p(
       -pos.x,
       -pos.y,
        pos.z
    );

    body_->applyForce(f, p);
}

void Body::applyCentralImpulse(gr::Vec3 impulse) {
    btVector3 i(
        impulse.x,
        impulse.y,
        impulse.z
    );

    body_->applyCentralImpulse(i);
}

void Body::applyImpulse(gr::Vec3 impulse, gr::Vec3 pos) {
    btVector3 i(
        impulse.x,
        impulse.y,
        impulse.z
    );

    btVector3 p(
       -pos.x,
       -pos.y,
        pos.z
    );

    body_->applyImpulse(i, p);
}

void Body::applyTorque(gr::Vec3 torque) {
    btVector3 t(
        torque.z,
        torque.y,
       -torque.x
    );

    body_->applyTorque(t);
}

void Body::applyTorqueImpulse(gr::Vec3 torque) {
    btVector3 t(
        torque.z,
        torque.y,
       -torque.x
    );

    body_->applyTorqueImpulse(t);
}

Body::~Body() {
    delete body_;
    delete shape_;
    delete motion_;
}

btRigidBody* Body::getRaw_() {
    return body_;
}

}