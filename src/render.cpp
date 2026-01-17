#include "granite/render.hpp"

#include "GLFW/glfw3.h"

namespace gr::internal {

    static const char* defaultVertex = R"(

    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform vec3 u_Position;

    void main() {
        gl_Position = vec4(aPos + u_Position, 1.0);
    }

    )";

    static const char* defaultFragment = R"(

    #version 330 core
    out vec4 FragColor;

    void main() {
        FragColor = vec4(1.0);
    }

    )";
}

namespace gr::Render{

void init(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;
}

void Mesh::upload(const float* vertices, uint32_t vertexCount){
    vertexCount_ = vertexCount;

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertexCount * 3 * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Mesh::draw(GLenum drawMode) const{
    glBindVertexArray(vao_);
    glDrawArrays(drawMode, 0, vertexCount_);
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

Mesh::Mesh(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
}

void RenderObject::draw(const Shader& shader) const{
    shader.use();

    GLint loc = glGetUniformLocation(shader.getProgram(), "u_Position");

    glUniform3f(
        loc,
        transform.position.x,
        transform.position.y,
        transform.position.z
    );

    mesh->draw();
}

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

};