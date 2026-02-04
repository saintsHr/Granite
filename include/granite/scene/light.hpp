#pragma once

#include "granite/core/vector.hpp"
#include "granite/core/color.hpp"

#include <unordered_map>

namespace gr::Scene {

using LightID = std::uint64_t;

class DirectionalLight {
public:
    gr::Vec3 direction;
    gr::Color3 color;
    float intensity;
    bool enabled;

    DirectionalLight() : 
        direction(0,0,0),
        color(1,1,1),
        intensity(1.0f),
        enabled(true) {}
private:

};

class PointLight {
public:
    gr::Vec3 position;
    gr::Color3 color;
    float intensity;
    float radius;
    bool enabled;

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
    gr::Color3 color;
    float intensity;
    bool enabled;

    AmbientLight() : 
        color(1,1,1),
        intensity(1.0f),
        enabled(true) {}
private:

};

class LightManager {
public:
    static LightID create(const PointLight& light);
    static LightID create(const DirectionalLight& light);

    static void destroyPointLight(LightID id);
    static void destroyDirectionalLight(LightID id);

    static PointLight* getPointLight(LightID id);
    static DirectionalLight* getDirectionalLight(LightID id);
private:
    static LightID nextID_;
    static std::unordered_map<LightID, PointLight> pointLights_;
    static std::unordered_map<LightID, DirectionalLight> directionalLights_;
};

}