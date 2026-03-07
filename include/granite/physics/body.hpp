#pragma once

#include "granite/core/vector.hpp"

namespace gr::Physics {

enum class BodyType {
    STATIC,
    DYNAMIC,
    KINEMATIC
};

class Body {
public:
    gr::Vec3 position = {0.0f, 0.0f, 0.0f};
    gr::Vec3 rotation = {0.0f, 0.0f, 0.0f};
    float mass = 1.0f;
    BodyType type = BodyType::DYNAMIC;
private:

};

}