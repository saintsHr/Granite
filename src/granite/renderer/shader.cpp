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

#include "granite/renderer/shader.hpp"
#include "granite/core/log.hpp"

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

namespace gr::internal {

auto checkShader = [](GLuint shader, const char* name) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        printf("SHADER COMPILE ERROR (%s):\n%s\n", name, log);
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::RENDERER,
            "Cannot compile %s shader",
            name
        );
    }
};

const char* defaultVertexShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform mat4 uLightSpace;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoord;
out vec4 vFragPosLightSpace;

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    vNormal = normalize(normalMatrix * aNormal);

    vTexCoord = aTexCoord;

    vFragPosLightSpace = uLightSpace * vec4(vFragPos, 1.0);

    gl_Position = uProjection * uView * worldPos;
}

// ------------------------------------------------------------------------------//

)glsl";

const char* defaultFragmentShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core

uniform vec3 uColor;
uniform float uShininess;
uniform float uOpacity;
uniform vec3 uCameraPos;
uniform sampler2D uTexture;
uniform sampler2D uShadowMap;
uniform bool uHasTexture;
uniform vec3 uSpecularColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;
in vec4 vFragPosLightSpace;

out vec4 vFragColor;

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS  32
#define MAX_DIRECTIONAL_LIGHTS 4

// ------------------------------------------------------------
// light structs
// ------------------------------------------------------------

struct GPUPointLight {
    vec3 position;
    float radius;

    vec3 color;
    float intensity;
};

struct GPUSpotLight {
    vec3 position;
    float radius;

    vec3 direction;
    float cutoff;

    vec3 color;
    float intensity;
};

struct GPUDirectionalLight {
    vec3 direction;
    float intensity;

    vec3 color;
    float _padding;
};

struct GPUAmbientLight {
    vec3 color;
    float intensity;
};

layout(std140) uniform LightBlock {
    GPUPointLight pointLights[MAX_POINT_LIGHTS];
    GPUSpotLight spotLights[MAX_SPOT_LIGHTS];
    GPUDirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
    GPUAmbientLight ambientLight;

    ivec4 counts;
};

// ------------------------------------------------------------
// utility
// ------------------------------------------------------------

float computeSpecBase(float NdotH) {
    return pow(NdotH, uShininess) * (uShininess + 2.0) / 8.0;
}

vec3 fresnelSchlick(vec3 F0, float VdotH) {
    return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
}

float distanceAttenuation(float distance) {
    return 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
}

float radiusFade(float radius, float distance) {
    return smoothstep(radius, radius * 0.9, distance);
}

float spotFactor(vec3 L, vec3 direction, float cutoff) {
    float theta = dot(-L, normalize(direction));
    float softness = (1.0 - cutoff) * 0.2;

    return smoothstep(
        cutoff - softness,
        cutoff,
        theta
    );
}

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (
        projCoords.x < 0.0 ||
        projCoords.x > 1.0 ||
        projCoords.y < 0.0 ||
        projCoords.y > 1.0
    ) {
        return 0.0;
    }

    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(
        0.05 * (1.0 - dot(normalize(vNormal), normalize(-directionalLights[0].direction))),
        0.005
    );

    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

// ------------------------------------------------------------
// lighting contribution
// ------------------------------------------------------------

void accumulateLight(
    vec3 L,
    vec3 V,
    vec3 N,
    vec3 lightColor,
    float intensity,
    float attenuation,
    inout vec3 diffuseAccum,
    inout vec3 specAccum
) {
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);
    if (NdotL <= 0.0) return;

    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    float specBase = computeSpecBase(NdotH);

    vec3 kS = fresnelSchlick(uSpecularColor, VdotH);
    vec3 kD = vec3(1.0);

    diffuseAccum += kD * NdotL * lightColor * intensity * attenuation;
    specAccum    += kS * specBase * lightColor * intensity * attenuation;
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(uCameraPos - vFragPos);

    vec3 diffuseAccum = ambientLight.color * ambientLight.intensity;
    vec3 specAccum = vec3(0.0);

    // --------------------------------------------------------
    // directional lights
    // --------------------------------------------------------

    for (int i = 0; i < counts.y; i++) {
        vec3 L = normalize(-directionalLights[i].direction);

        accumulateLight(
            L,
            V,
            N,
            directionalLights[i].color,
            directionalLights[i].intensity,
            1.0,
            diffuseAccum,
            specAccum
        );
    }

    // --------------------------------------------------------
    // point lights
    // --------------------------------------------------------

    for (int i = 0; i < counts.x; i++) {
        vec3 lightVec = pointLights[i].position - vFragPos;
        float distance = max(length(lightVec), 0.001);

        vec3 L = lightVec / distance;

        float attenuation =
            distanceAttenuation(distance) *
            radiusFade(pointLights[i].radius, distance);

        if (attenuation <= 0.0) continue;

        accumulateLight(
            L,
            V,
            N,
            pointLights[i].color,
            pointLights[i].intensity,
            attenuation,
            diffuseAccum,
            specAccum
        );
    }

    // --------------------------------------------------------
    // spot lights
    // --------------------------------------------------------

    for (int i = 0; i < counts.z; i++) {
        vec3 lightVec = spotLights[i].position - vFragPos;
        float distance = max(length(lightVec), 0.001);

        vec3 L = lightVec / distance;

        float attenuation =
            distanceAttenuation(distance) *
            radiusFade(spotLights[i].radius, distance);

        if (attenuation <= 0.0) continue;

        float spotIntensity =
            spotFactor(L, spotLights[i].direction, spotLights[i].cutoff);

        if (spotIntensity <= 0.0) continue;

        accumulateLight(
            L,
            V,
            N,
            spotLights[i].color,
            spotLights[i].intensity,
            attenuation * spotIntensity,
            diffuseAccum,
            specAccum
        );
    }

    float shadow = ShadowCalculation(vFragPosLightSpace);
    vec3 baseColor = uHasTexture ? texture(uTexture, vTexCoord).rgb : uColor;
    vec3 lighting = diffuseAccum * baseColor + specAccum;
    vec3 finalColor = lighting * (1.0 - shadow);

    finalColor = finalColor / (finalColor + vec3(1.0));

    vFragColor = vec4(finalColor, uOpacity);
}

// ------------------------------------------------------------------------------//

)glsl";

}

namespace gr::Renderer {

const Shader* currentShader = nullptr;

Shader::Shader() {
    const char* vertexSource = gr::internal::defaultVertexShader;
    const char* fragmentSource = gr::internal::defaultFragmentShader;

    // creates & compiles shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    gr::internal::checkShader(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    gr::internal::checkShader(fragmentShader, "FRAGMENT");

    // links shaders into program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // checks link errors
    GLint linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, log);
        printf("PROGRAM LINK ERROR:\n%s\n", log);
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::RENDERER,
            "Cannot link shaders"
        );
    }

    // light block binding
    GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "LightBlock");
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(shaderProgram, blockIndex, 0);
    }

    program_ = shaderProgram;

    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string &vertexStr, const std::string &fragmentStr) {
    const char* vertexSource   = vertexStr.c_str();
    const char* fragmentSource = fragmentStr.c_str();

    // creates & compiles shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    gr::internal::checkShader(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    gr::internal::checkShader(fragmentShader, "FRAGMENT");

    // links shaders into program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // checks link errors
    GLint linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, log);
        printf("PROGRAM LINK ERROR:\n%s\n", log);
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::RENDERER,
            "Cannot link shaders"
        );
    }

    // light block binding
    GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "LightBlock");
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(shaderProgram, blockIndex, 0);
    }

    program_ = shaderProgram;

    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(program_);
}

std::string Shader::load_(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::RENDERER,
            "Cannot read shader file %s",
            filename.c_str()
        );
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();
    return buffer.str();
}

void Shader::use() const {
    glUseProgram(program_);
    currentShader = this;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    GLint loc = glGetUniformLocation(program_, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::getProgram() const {
    return program_;
}

GLint Shader::getPLoc() const {
    return pLoc_;
}

GLint Shader::getVLoc() const {
    return vLoc_;
}

void Shader::setPLoc(GLint loc) {
    pLoc_ = loc;
}

void Shader::setVLoc(GLint loc) {
    vLoc_ = loc;
}

}