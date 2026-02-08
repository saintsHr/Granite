#include "granite/render/shader.hpp"
#include "granite/core/log.hpp"

#include <cstddef>
#include <cstdio>

namespace gr::internal {

auto checkShader = [](GLuint shader, const char* name){
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

static const char* defaultVertex = R"(

    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    uniform mat4 uProjection;
    uniform mat4 uView;
    uniform mat4 uModel;

    out vec3 vNormal;
    out vec3 vFragPos;

    void main(){
        mat3 normalMatrix = transpose(inverse(mat3(uModel)));
        vNormal = normalize(normalMatrix * aNormal);

        vFragPos = vec3(uModel * vec4(aPos, 1.0));
        gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    }

)";

static const char* defaultFragment = R"(

    #version 330 core

    uniform vec3 uColor;
    uniform float uShininess;
    uniform vec3 uCameraPos;

    in vec3 vNormal;
    in vec3 vFragPos;
    out vec4 FragColor;

    #define MAX_POINT_LIGHTS 32
    #define MAX_DIRECTIONAL_LIGHTS 4

    struct GPUPointLight {
        vec3 position;
        float radius;

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
        GPUDirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
        GPUAmbientLight ambientLight;

        ivec4 counts;
    };

    void main(){
        vec3 N = normalize(vNormal);
        vec3 V = normalize(uCameraPos - vFragPos);

        vec3 result = ambientLight.color * ambientLight.intensity * uColor;

        for (int i = 0; i < counts.y; i++) {
            // directional
            vec3 L = normalize(-directionalLights[i].direction);
            float diff = max(dot(N, L), 0.0);
            result += diff * directionalLights[i].color * directionalLights[i].intensity;

            // specular
            vec3 H = normalize(L + V);
            float spec = pow(max(dot(N, H), 0.0), uShininess);
            result += spec * directionalLights[i].color * directionalLights[i].intensity;
        }

        FragColor = vec4(result, 1.0);
    }

)";

}

namespace gr::Render{

Shader::Shader(const char* vertexSource, const char* fragmentSource){
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

Shader::Shader() : Shader(gr::internal::defaultVertex, gr::internal::defaultFragment){}

Shader::~Shader(){
    glDeleteProgram(program_);
}

void Shader::use() const{
    glUseProgram(program_);
}

GLuint Shader::getProgram() const{
    return program_;
}

GLint Shader::getPLoc() const{
    return pLoc_;
}

GLint Shader::getVLoc() const{
    return vLoc_;
}

void Shader::setPLoc(GLint loc){
    pLoc_ = loc;
}

void Shader::setVLoc(GLint loc){
    vLoc_ = loc;
}

}