#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vector_relational.hpp>

#include "granite/render/render.hpp"
#include "granite/scene/light.hpp"

namespace gr::Render{

FrameContext gFrame;

GLuint lightUBO;

void init(){
    // loads GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // creates light UBO
    glGenBuffers(1, &lightUBO);

    // binds light UBO
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
    glBufferData(
        GL_UNIFORM_BUFFER,
        sizeof(gr::Scene::GPULightBlock),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    // binds light UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUBO);
}

void beginFrame(const gr::Scene::Camera& camera){
    gFrame.view = camera.getView();
    gFrame.projection = camera.getProjection();

    // creates empty light block
    gr::Scene::GPULightBlock block{};
    block.counts = glm::ivec4(0);

    // copy point lights to block
    int pointIndex = 0;
    for (auto& [id, light] : gr::Scene::LightManager::getPointLights()) {
        if (!light.enabled) continue;
        if (pointIndex >= gr::Scene::MAX_POINT_LIGHTS) break;

        auto& gpu = block.pointLights[pointIndex++];

        gpu.position  = { light.position.x, light.position.y, light.position.z };
        gpu.color     = { light.color.r, light.color.g, light.color.b };
        gpu.intensity = light.intensity;
        gpu.radius    = light.radius;
    }
    block.counts.x = pointIndex;

    // copy directional lights to block
    int dirIndex = 0;
    for (auto& [id, light] : gr::Scene::LightManager::getDirectionalLights()) {
        if (!light.enabled) continue;
        if (dirIndex >= gr::Scene::MAX_DIRECTIONAL_LIGHTS) break;

        auto& gpu = block.directionalLights[dirIndex++];

        gpu.direction = normalize(glm::vec3(
            light.direction.x,
            light.direction.y,
            light.direction.z
        ));

        gpu.color     = { light.color.r, light.color.g, light.color.b };
        gpu.intensity = light.intensity;
    }
    block.counts.y = dirIndex;

    // copy ambient light to block
    auto* amb = gr::Scene::LightManager::getAmbientLight();
    if (amb && amb->enabled) {
        block.ambientLight.color = {
            amb->color.r,
            amb->color.g,
            amb->color.b
        };
        block.ambientLight.intensity = amb->intensity;
    }

    // sends block to UBO
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(block), &block);
}

};