#include "granite/render/shader.hpp"
#include "granite/core/log.hpp"

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>

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
            gr::internal::Module::RENDER,
            "Cannot compile %s shader",
            name
        );
    }
};

const char* defaultVertexShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoord;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(uModel)));
    vNormal = normalize(normalMatrix * aNormal);

    vFragPos = vec3(uModel * vec4(aPos, 1.0));
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    vTexCoord = aTexCoord;
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
uniform bool uHasTexture;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

out vec4 vFragColor;

#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS  32
#define MAX_DIRECTIONAL_LIGHTS 4

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

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(uCameraPos - vFragPos);
    const float F0 = 0.04;

    vec3 diffuseAccum = ambientLight.color * ambientLight.intensity;
    vec3 specAccum = vec3(0.0);

    // directional lights
    for (int i = 0; i < counts.y; i++) {
        vec3 L = normalize(-directionalLights[i].direction);
        vec3 H = normalize(L + V);

        vec3 lightColor = directionalLights[i].color;
        float intensity = directionalLights[i].intensity;

        float NdotH = max(dot(N, H), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if (NdotL <= 0.0) continue;

        float specBase = pow(NdotH, uShininess);

        // Fresnel (Schlick)
        float kS = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

        // Energy conservation
        float kD = 1.0 - kS;

        // Diffuse
        diffuseAccum += kD * NdotL * lightColor * intensity;

        // Specular
        specAccum += kS * specBase * lightColor * intensity;
    }

    // point lights
    for (int i = 0; i < counts.x; i++) {
        vec3 lightVec = pointLights[i].position - vFragPos;
        float distance = max(length(lightVec), 0.001);

        vec3 L = lightVec / distance;
        vec3 H = normalize(L + V);

        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        float radiusFade = smoothstep(
            pointLights[i].radius,
            pointLights[i].radius * 0.9,
            distance
        );
        attenuation *= radiusFade;

        if (attenuation <= 0.0) continue;

        vec3 lightColor = pointLights[i].color;
        float intensity = pointLights[i].intensity;

        float NdotH = max(dot(N, H), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if (NdotL <= 0.0) continue;

        float specBase = pow(NdotH, uShininess);

        // Fresnel (Schlick)
        float kS = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
        float kD = 1.0 - kS;

        // Diffuse
        diffuseAccum += kD * NdotL * lightColor * intensity * attenuation;

        // Specular
        specAccum += kS * specBase * lightColor * intensity * attenuation;
    }

    // spot lights
    for (int i = 0; i < counts.z; i++) {

        vec3 lightVec = spotLights[i].position - vFragPos;
        float distance = max(length(lightVec), 0.001);

        vec3 L = lightVec / distance;
        vec3 H = normalize(L + V);

        // attenuation
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        float radiusFade = smoothstep(
            spotLights[i].radius,
            spotLights[i].radius * 0.9,
            distance
        );
        attenuation *= radiusFade;

        if (attenuation <= 0.0) continue;

        float theta = dot(L, normalize(-spotLights[i].direction));
        float softness = (1.0 - spotLights[i].cutoff) * 0.2;
        float spotIntensity = smoothstep(
            spotLights[i].cutoff - softness,
            spotLights[i].cutoff,
            theta
        );

        if (spotIntensity <= 0.0) continue;

        float NdotH = max(dot(N, H), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if (NdotL <= 0.0) continue;

        float specBase = pow(NdotH, uShininess);

        vec3 lightColor = spotLights[i].color;
        float intensity = spotLights[i].intensity;

        // Fresnel (Schlick)
        float kS = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
        float kD = 1.0 - kS;

        // Diffuse
        diffuseAccum += kD * NdotL * lightColor * intensity * attenuation * spotIntensity;

        // Specular
        specAccum += kS * specBase * lightColor * intensity * attenuation * spotIntensity;
    }

    vec3 baseColor = uHasTexture ? texture(uTexture, vTexCoord).rgb : uColor;

    vec3 finalColor = diffuseAccum * baseColor + specAccum;
    finalColor = finalColor / (finalColor + vec3(1.0));

    vFragColor = vec4(finalColor, uOpacity);
}

// ------------------------------------------------------------------------------//

)glsl";

}

namespace gr::Render {

const Shader* currentShader = nullptr;

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile) {
    // opens & reads shaders
    std::string vertexRead, fragmentRead;
    const char* vertexSource;
    const char* fragmentSource;

    vertexRead = load_(vertexFile);
    fragmentRead = load_(fragmentFile);

    if (vertexRead != "" && fragmentRead != "") {
        vertexSource = vertexRead.c_str();
        fragmentSource = fragmentRead.c_str();
    } else {
        vertexSource = gr::internal::defaultVertexShader;
        fragmentSource = gr::internal::defaultFragmentShader;
    }

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
            gr::internal::Module::RENDER,
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
            gr::internal::Module::RENDER,
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
            gr::internal::Module::RENDER,
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