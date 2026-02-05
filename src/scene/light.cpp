#include "granite/scene/light.hpp"

namespace gr::Scene {

LightID LightManager::nextID_ = 1;

std::unordered_map<LightID, PointLight> LightManager::pointLights_;
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

void LightManager::destroyPointLight(LightID id) {
    pointLights_.erase(id);
}

void LightManager::destroyDirectionalLight(LightID id) {
    directionalLights_.erase(id);
}

PointLight* LightManager::getPointLight(LightID id) {
    auto it = pointLights_.find(id);
    if (it == pointLights_.end()) return nullptr;
    return &it->second;
}

DirectionalLight* LightManager::getDirectionalLight(LightID id) {
    auto it = directionalLights_.find(id);
    if (it == directionalLights_.end()) return nullptr;
    return &it->second;
}

AmbientLight*  LightManager::getAmbientLight(){
    return &ambientLight_;
}

}