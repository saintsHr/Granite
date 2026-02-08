#pragma once

#include "granite/core/vector.hpp"
#include "granite/core/color.hpp"

#include <unordered_map>

namespace gr::Scene {

constexpr int MAX_POINT_LIGHTS = 32;
constexpr int MAX_SPOT_LIGHTS = 32;
constexpr int MAX_DIRECTIONAL_LIGHTS = 4;

using LightID = std::uint64_t;

struct alignas(16) GPUPointLight {
    glm::vec3 position;
    float radius;

    glm::vec3 color;
    float intensity;
};

struct alignas(16) GPUSpotLight {
    glm::vec3 position;
    float radius;

    glm::vec3 direction;
    float cutoff;

    glm::vec3 color;
    float intensity;
};

struct alignas(16) GPUDirectionalLight {
    glm::vec3 direction;
    float intensity;

    glm::vec3 color;
    float padding;
};

struct alignas(16) GPUAmbientLight {
    glm::vec3 color;
    float intensity;
};

struct alignas(16) GPULightBlock {
    GPUPointLight pointLights[MAX_POINT_LIGHTS];
    GPUSpotLight spotLights[MAX_SPOT_LIGHTS];
    GPUDirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
    GPUAmbientLight ambientLight;

    glm::ivec4 counts;
};

static_assert(sizeof(GPUPointLight) == 32, "GPUPointLight layout mismatch");
static_assert(sizeof(GPUDirectionalLight) == 32, "GPUDirectionalLight layout mismatch");
static_assert(sizeof(GPUAmbientLight) == 16, "GPUAmbientLight layout mismatch");

class DirectionalLight {
public:
    gr::Vec3 direction;
    gr::Color3 color;
    float intensity;
    bool enabled;

    DirectionalLight() : 
        direction(0,-1,0),
        color(1,1,1),
        intensity(0.5f),
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
        intensity(0.5f),
        radius(5.0f),
        enabled(true) {}
private:

};

class SpotLight {
public:
    gr::Vec3 position;
    gr::Vec3 direction;
    gr::Color3 color;
    float radius;
    float cutoff;
    float intensity;
    bool enabled;

    SpotLight() : 
        position(0,0,0),
        direction(0, -1, 0),
        color(1,1,1),
        radius(5.0f),
        cutoff(static_cast<float>(cos(45.0))),
        intensity(0.5f),
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
        intensity(0.5f),
        enabled(true) {}
private:

};

class LightManager {
public:
    static LightID create(const PointLight& light);
    static LightID create(const SpotLight& light);
    static LightID create(const DirectionalLight& light);

    static void destroyPointLight(LightID id);
    static void destroySpotLight(LightID id);
    static void destroyDirectionalLight(LightID id);

    static PointLight* getPointLight(LightID id);
    static SpotLight* getSpotLight(LightID id);
    static DirectionalLight* getDirectionalLight(LightID id);
    static AmbientLight* getAmbientLight();

    static const std::unordered_map<LightID, PointLight>& getPointLights();
    static const std::unordered_map<LightID, SpotLight>& getSpotLights();
    static const std::unordered_map<LightID, DirectionalLight>& getDirectionalLights();
private:
    static LightID nextID_;
    static std::unordered_map<LightID, PointLight> pointLights_;
    static std::unordered_map<LightID, SpotLight> spotLights_;
    static std::unordered_map<LightID, DirectionalLight> directionalLights_;
    static AmbientLight ambientLight_;
};

}