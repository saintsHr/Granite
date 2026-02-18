#include "granite/scene/light.hpp"

#include <cmath>

namespace gr::Scene {

LightID LightManager::nextID_ = 1;

std::unordered_map<LightID, PointLight> LightManager::pointLights_;
std::unordered_map<LightID, SpotLight> LightManager::spotLights_;
std::unordered_map<LightID, DirectionalLight> LightManager::directionalLights_;
AmbientLight LightManager::ambientLight_;

LightID LightManager::create(const PointLight& light) {
    LightID id = nextID_++;
    pointLights_.emplace(id, light);
    return id;
}

LightID LightManager::create(const DirectionalLight& light) {
    LightID id = nextID_++;
    directionalLights_.emplace(id, light);
    return id;
}

LightID LightManager::create(const SpotLight& light) {
    SpotLight light_ = light;
    light_.cutoff = std::cos(light.cutoff);
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