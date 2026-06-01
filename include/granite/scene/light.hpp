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
#include "granite/core/color.hpp"

#include <cmath>
#include <cstdint>
#include <unordered_map>

namespace gr::Scene {

constexpr int MAX_POINT_LIGHTS = 32;
constexpr int MAX_SPOT_LIGHTS = 32;
constexpr int MAX_DIRECTIONAL_LIGHTS = 4;

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
        cutoff(static_cast<float>(std::cos(45.0))),
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
        intensity(0.05f),
        enabled(true) {}
private:

};

class LightManager {
public:
    static uint64_t create(const PointLight& light);
    static uint64_t create(const SpotLight& light);
    static uint64_t create(const DirectionalLight& light);

    static void destroyPointLight(uint64_t id);
    static void destroySpotLight(uint64_t id);
    static void destroyDirectionalLight(uint64_t id);

    static PointLight* getPointLight(uint64_t id);
    static SpotLight* getSpotLight(uint64_t id);
    static DirectionalLight* getDirectionalLight(uint64_t id);
    static AmbientLight* getAmbientLight();

    static const std::unordered_map<uint64_t, PointLight>& getPointLights();
    static const std::unordered_map<uint64_t, SpotLight>& getSpotLights();
    static const std::unordered_map<uint64_t, DirectionalLight>& getDirectionalLights();
private:
    static uint64_t nextID_;
    static std::unordered_map<uint64_t, PointLight> pointLights_;
    static std::unordered_map<uint64_t, SpotLight> spotLights_;
    static std::unordered_map<uint64_t, DirectionalLight> directionalLights_;
    static AmbientLight ambientLight_;
};

}
