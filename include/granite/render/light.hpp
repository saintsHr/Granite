#pragma once

#include "granite/core/vector.hpp"
#include "granite/core/color.hpp"

namespace gr::Render {

class DirectionalLight {
public:
    bool enabled;
    gr::Vec3 direction;
    gr::Color3 color;
    float intensity;

    DirectionalLight() : 
        direction(0,0,0),
        color(1,1,1),
        intensity(1.0f),
        enabled(true) {}
private:

};

class PointLight {
public:
    bool enabled;
    gr::Vec3 position;
    gr::Color3 color;
    float intensity;
    float radius;

    PointLight() : 
        position(0,0,0),
        color(1,1,1),
        intensity(1.0f),
        radius(5.0f),
        enabled(true) {}
private:

};

class AmbientLight {
public:
    bool enabled;
    gr::Color3 color;
    float intensity;

    AmbientLight() : 
        color(1,1,1),
        intensity(1.0f),
        enabled(true) {}
private:

};

}