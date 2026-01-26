#include "granite/render/shader.hpp"
#include <cstddef>

namespace gr::internal {

    static const char* defaultVertex = R"(

    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    uniform mat4 uProjection;
    uniform mat4 uView;
    uniform mat4 uModel;

    out vec3 vNormal;

    void main(){
        mat3 normalMatrix = transpose(inverse(mat3(uModel)));
        vNormal = normalize(normalMatrix * aNormal);
        gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    }

    )";

    static const char* defaultFragment = R"(

    #version 330 core

    uniform vec3 uColor;

    in vec3 vNormal;
    out vec4 FragColor;

    void main(){
        FragColor = vec4(uColor, 1.0);
    }

    )";
}

namespace gr::Render{

Shader::Shader(const char* vertexSource, const char* fragmentSource){
    // creates & compiles shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // links shaders into program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

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