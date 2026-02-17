#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vector_relational.hpp>

#include "granite/render/render.hpp"
#include "granite/scene/light.hpp"
#include "granite/core/log.hpp"
#include <algorithm>

namespace gr::Render {

FrameContext gFrame;

GLuint lightUBO = 0;

void init() {
    bool success = true;

    auto checkGL = [&success]() {
        bool err = false;
        while (glGetError() != GL_NO_ERROR) {
            err = true;
        }
        if (err) success = false;
    };

    // loads GLAD
    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)
    )) {
        success = false;
    }

    // configs OpenGL
    while (glGetError() != GL_NO_ERROR) {}
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGL();

    // creates light UBO
    while (glGetError() != GL_NO_ERROR) {}
    glGenBuffers(1, &lightUBO);
    if (lightUBO == 0) success = false;
    checkGL();

    // binds light UBO
    while (glGetError() != GL_NO_ERROR) {}
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
    glBufferData(
        GL_UNIFORM_BUFFER,
        sizeof(gr::Scene::GPULightBlock),
        nullptr,
        GL_DYNAMIC_DRAW
    );
    checkGL();

    // binds light UBO
    while (glGetError() != GL_NO_ERROR) {}
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUBO);
    checkGL();

    if (!success) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::RENDER,
            "Cannot initialize renderer"
        );
    }

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::RENDER,
        "Renderer Initialized (OpenGL)"
    );
}

void beginFrame(const gr::Scene::Camera& camera){
    gFrame.view = camera.getView();
    gFrame.projection = camera.getProjection();
    gFrame.cameraPos = camera.pos;

    // sends camera position to shader
    if (gr::Render::currentShader) {
        GLint camLoc = glGetUniformLocation(currentShader->getProgram(), "uCameraPos");
        glUniform3f(camLoc, camera.pos.x, camera.pos.y, camera.pos.z);
    }

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

    // copy point lights to block
    int spotIndex = 0;
    for (auto& [id, light] : gr::Scene::LightManager::getSpotLights()) {
        if (!light.enabled) continue;
        if (spotIndex >= gr::Scene::MAX_SPOT_LIGHTS) break;

        auto& gpu = block.spotLights[spotIndex++];

        gpu.position  = { light.position.x, light.position.y, light.position.z };
        gpu.direction = { light.direction.x, light.direction.y, light.direction.z };
        gpu.color     = { light.color.r, light.color.g, light.color.b };
        gpu.intensity = light.intensity;
        gpu.radius    = light.radius;
        gpu.cutoff    = light.cutoff;
    }
    block.counts.z = spotIndex;

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

std::vector<gr::Scene::RenderObject> opaqueObjects;
std::vector<gr::Scene::RenderObject> transparentObjects;

void addToQueue(const gr::Scene::RenderObject& obj) {
    if (obj.material.opacity < 1.0f) {
        transparentObjects.push_back(obj);
    } else {
        opaqueObjects.push_back(obj);
    }
}

void endFrame() {
    for (size_t i = 0; i < opaqueObjects.size(); i++){
        opaqueObjects[i].draw();
    }

    std::sort(
        transparentObjects.begin(),
        transparentObjects.end(),
        [](const gr::Scene::RenderObject& a, const gr::Scene::RenderObject& b) {
            float da = (gr::Render::gFrame.cameraPos - a.transform.position).length();
            float db = (gr::Render::gFrame.cameraPos - b.transform.position).length();
            return da > db;
        }
    );

    for (size_t i = 0; i < transparentObjects.size(); i++){
        transparentObjects[i].draw();
    }

    opaqueObjects.clear();
    transparentObjects.clear();
}

}