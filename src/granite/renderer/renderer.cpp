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

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vector_relational.hpp>
#include <glm/ext.hpp>

#include "granite/renderer/renderer.hpp"
#include "granite/scene/light.hpp"
#include "granite/core/log.hpp"
#include <algorithm>

namespace gr::Renderer {

FrameContext gFrame;

GLuint lightUBO = 0;

GLuint depthMapFBO = 0;
GLuint depthMap    = 0;
const GLsizei SHADOW_WIDTH  = 1024;
const GLsizei SHADOW_HEIGHT = 1024;

void calcLightSpace() {
    glm::vec3 camPos = glm::vec3(
        gFrame.cameraPos.x,
        gFrame.cameraPos.y,
        gFrame.cameraPos.z
    );
    
    glm::vec3 camForward = -glm::vec3(gFrame.view[2]);
    glm::vec3 frustumCenter = camPos + camForward * 20.0f;

    auto& dirLights = gr::Scene::LightManager::getDirectionalLights();
    if (!dirLights.empty()) {
        gFrame.lightSpaces.resize(dirLights.size());

        int i = 0;
        for (auto& [id, light] : dirLights) {
            glm::vec3 lightDir = glm::normalize(glm::vec3(
                light.direction.x,
                light.direction.y,
                light.direction.z
            ));

            glm::vec3 lightPos = frustumCenter - lightDir * 30.0f;

            glm::mat4 lightView = glm::lookAt(
                lightPos,
                frustumCenter,
                glm::vec3(0,1,0)
            );

            float size = 25.0f;

            glm::mat4 lightProj = glm::ortho(
                -size, size,
                -size, size,
                -50.0f, 50.0f
            );

            gFrame.lightSpaces[i++] = lightProj * lightView;
        }
    }
}

void init() {
    bool success = true;

    auto checkGL = [&success]() {
        bool err = false;
        while (glGetError() != GL_NO_ERROR) {
            err = true;
        }
        if (err) success = false;
    };

    // --- loads GLAD ---
    if (!gladLoadGLLoader(
        reinterpret_cast<GLADloadproc>(glfwGetProcAddress)
    )) {
        success = false;
    }

    // --- configs OpenGL ---
    while (glGetError() != GL_NO_ERROR) {}

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    checkGL();

    // --- creates light UBO ---
    while (glGetError() != GL_NO_ERROR) {}
    glGenBuffers(1, &lightUBO);
    if (lightUBO == 0) success = false;
    checkGL();

    // --- binds light UBO ---
    while (glGetError() != GL_NO_ERROR) {}
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
    glBufferData(
        GL_UNIFORM_BUFFER,
        sizeof(gr::Scene::GPULightBlock),
        nullptr,
        GL_DYNAMIC_DRAW
    );
    checkGL();

    // --- binds light UBO ---
    while (glGetError() != GL_NO_ERROR) {}
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUBO);
    checkGL();

    // --- generates depth map & depth map FBO ---
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);

    // --- creates depth map texture ---
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT, 
        SHADOW_WIDTH,
        SHADOW_HEIGHT,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // --- attach depth map texture to depth map FBO ---
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depthMap,
        0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) success = false;
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!success) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::RENDERER,
            "Cannot initialize renderer"
        );
    }

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::RENDERER,
        "Renderer Initialized (OpenGL)"
    );
}

void beginFrame(const gr::Scene::Camera& camera){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    gFrame.view = camera.getView();
    gFrame.projection = camera.getProjection();
    gFrame.cameraPos = camera.position;

    // sends camera position to shader
    if (gr::Renderer::currentShader) {
        GLint camLoc = glGetUniformLocation(currentShader->getProgram(), "uCameraPos");
        glUniform3f(camLoc, camera.position.x, camera.position.y, camera.position.z);
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

std::vector<gr::Renderer::RenderObject> opaqueObjects;
std::vector<gr::Renderer::RenderObject> transparentObjects;

void addToQueue(const gr::Renderer::RenderObject& obj) {
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
        [](const gr::Renderer::RenderObject& a, const gr::Renderer::RenderObject& b) {
            float da = (gr::Renderer::gFrame.cameraPos - a.transform.position).length();
            float db = (gr::Renderer::gFrame.cameraPos - b.transform.position).length();
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