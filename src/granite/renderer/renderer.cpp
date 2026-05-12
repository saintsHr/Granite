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
#include <algorithm>

#include "granite/renderer/renderer.hpp"
#include "granite/renderer/shader.hpp"
#include "granite/scene/light.hpp"
#include "granite/core/log.hpp"

namespace gr::Renderer {

GLuint lightUBO = 0;

FrameContext   gFrame;
RendererConfig gConfig;

std::shared_ptr<Shader> postShader = nullptr;
GLuint gPostFBO = 0;
GLuint gPostRBO = 0;
GLuint gPostTex = 0;

const char* postVertexShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core

layout(location = 0) in vec2 lPosition;
layout(location = 1) in vec2 lUV;

out vec2 UV;

void main() {
    UV = lUV;
    gl_Position = vec4(lPosition, 0.0, 1.0);
}

// ------------------------------------------------------------------------------//

)glsl";

const char* postFragmentShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core

uniform sampler2D uTexture;

uniform float uDitherLevels;
uniform float uQuantizationLevels;
uniform float uDirtyNoiseStrenght;

in vec2 UV;
out vec4 fragColor;

float bayer4x4(vec2 p) {
    int x = int(mod(p.x, 4.0));
    int y = int(mod(p.y, 4.0));

    int index = x + y * 4;

    const float m[16] = float[](
         0.0,  8.0,  2.0, 10.0,
        12.0,  4.0, 14.0,  6.0,
         3.0, 11.0,  1.0,  9.0,
        15.0,  7.0, 13.0,  5.0
    );

    return m[index] / 16.0;
}

float random(vec2 co) {
    return fract(
        sin(dot(co, vec2(12.9898,78.233))) * 43758.5453
    );
}

void main() {
    vec3 finalColor = texture(uTexture, UV).rgb;

    // --- Dither --- //
    if (uDitherLevels > 0.0f) {
        float dither = bayer4x4(gl_FragCoord.xy);
        finalColor += (dither - 0.5) / uDitherLevels;
    }

    // --- Quantization --- //
    if (uQuantizationLevels > 0.0f) {
        finalColor = floor(
            finalColor * uQuantizationLevels
        ) / uQuantizationLevels;
    }

    // --- Dirty Noise --- //
    if (uDirtyNoiseStrenght > 0.0f) {
        float noise = random(floor(gl_FragCoord.xy));
        finalColor += (noise - 0.5) * uDirtyNoiseStrenght;
    }

    finalColor = clamp(finalColor, 0.0, 1.0);
    fragColor  = vec4(finalColor, 1.0);
}

// ------------------------------------------------------------------------------//

)glsl";

static void drawPostQuad(GLuint tex) {
    static GLuint vao = 0;
    static GLuint vbo = 0;

    if (vao == 0) {
        float vertices[] = {
            -1, -1,    0, 0,
             1, -1,    1, 0,
             1,  1,    1, 1,
            -1,  1,    0, 1
        };

        GLuint indices[] = {0,1,2, 2,3,0};

        GLuint ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    postShader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    postShader->setFloat1("uQuantizationLevels", gConfig.quantizationLevels);
    postShader->setFloat1("uDitherLevels", gConfig.ditherLevels);
    postShader->setFloat1("uDirtyNoiseStrenght", gConfig.dirtyNoiseStrenght);
    postShader->setInt1("uTexture", 0);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void init() {
    gr::Renderer::RendererConfig rcfg;
    init(rcfg);
}

void init(const gr::Renderer::RendererConfig& cfg) {
    gConfig = cfg;

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

    if (!success) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::RENDERER,
            "Cannot initialize renderer."
        );
    }

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::RENDERER,
        "Renderer Initialized (OpenGL)."
    );

    // --- Post-Process Setup ---
    postShader = std::make_shared<gr::Renderer::Shader>(
        postVertexShader,
        postFragmentShader
    );

    glGenFramebuffers(1, &gPostFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, gPostFBO);

    glGenTextures(1, &gPostTex);
    glBindTexture(GL_TEXTURE_2D, gPostTex);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_SRGB8_ALPHA8,
        gConfig.resolution.x,
        gConfig.resolution.y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(gConfig.filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(gConfig.filter));

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        gPostTex,
        0
    );

    glGenRenderbuffers(1, &gPostRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, gPostRBO);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        gConfig.resolution.x,
        gConfig.resolution.y
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        gPostRBO
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void beginFrame(const gr::Scene::Camera& camera){
    glBindFramebuffer(GL_FRAMEBUFFER, gPostFBO);
    glViewport(0, 0, gConfig.resolution.x, gConfig.resolution.y);

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

void endFrame(const gr::Scene::Camera& camera) {
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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, camera.aspect.x, camera.aspect.y);

    glDisable(GL_DEPTH_TEST);
    drawPostQuad(gPostTex);
    glEnable(GL_DEPTH_TEST);
}

}
