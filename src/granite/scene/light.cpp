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

#include "granite/scene/light.hpp"

#include <cmath>

namespace gr::Scene {

LightID LightManager::nextID_ = 1;

std::unordered_map<LightID, PointLight> LightManager::pointLights_;
std::unordered_map<LightID, SpotLight> LightManager::spotLights_;
std::unordered_map<LightID, DirectionalLight> LightManager::directionalLights_;
AmbientLight LightManager::ambientLight_;

LightID LightManager::create(const PointLight& light) {
    PointLight light_ = light;
    light_.color = {
        light_.color.r / 255,
        light_.color.g / 255,
        light_.color.b / 255
    };

    LightID id = nextID_++;
    pointLights_.emplace(id, light_);
    return id;
}

LightID LightManager::create(const DirectionalLight& light) {
    DirectionalLight light_ = light;
    light_.color = {
        light_.color.r / 255,
        light_.color.g / 255,
        light_.color.b / 255
    };

    LightID id = nextID_++;
    directionalLights_.emplace(id, light_);
    return id;
}

LightID LightManager::create(const SpotLight& light) {
    SpotLight light_ = light;
    light_.color = {
        light_.color.r / 255,
        light_.color.g / 255,
        light_.color.b / 255
    };
    light_.cutoff = std::cos(glm::radians(light.cutoff));

    LightID id = nextID_++;
    spotLights_.emplace(id, light_);
    return id;
}

void LightManager::destroyPointLight(LightID id) {
    pointLights_.erase(id);
}

void LightManager::destroySpotLight(LightID id) {
    spotLights_.erase(id);
}

void LightManager::destroyDirectionalLight(LightID id) {
    directionalLights_.erase(id);
}

PointLight* LightManager::getPointLight(LightID id) {
    auto it = pointLights_.find(id);
    if (it == pointLights_.end()) return nullptr;
    return &it->second;
}

SpotLight* LightManager::getSpotLight(LightID id) {
    auto it = spotLights_.find(id);
    if (it == spotLights_.end()) return nullptr;
    return &it->second;
}

DirectionalLight* LightManager::getDirectionalLight(LightID id) {
    auto it = directionalLights_.find(id);
    if (it == directionalLights_.end()) return nullptr;
    return &it->second;
}

AmbientLight* LightManager::getAmbientLight(){
    return &ambientLight_;
}

const std::unordered_map<LightID, PointLight>& LightManager::getPointLights() {
    return pointLights_;
}

const std::unordered_map<LightID, SpotLight>& LightManager::getSpotLights() {
    return spotLights_;
}

const std::unordered_map<LightID, DirectionalLight>& LightManager::getDirectionalLights() {
    return directionalLights_;
}

}