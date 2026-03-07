#include "granite/physics/body.hpp"

namespace gr::Physics {

void Body::build() {
    btTransform btTransform;
    btTransform.setIdentity();
    btTransform.setOrigin(btVector3(
        static_cast<btScalar>(transform.position.x),
        static_cast<btScalar>(transform.position.y),
        static_cast<btScalar>(transform.position.z)
    ));

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
        static_cast<float>(pitch),
        static_cast<float>(yaw),
        static_cast<float>(roll)
    };
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