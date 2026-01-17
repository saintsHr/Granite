#include "granite/render.hpp"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gr::internal {

    static const char* defaultVertex = R"(

    #version 330 core
    layout(location = 0) in vec3 aPos;

    uniform mat4 uProjection;
    uniform mat4 uView;
    uniform mat4 uModel;

    void main(){
        gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    }

    )";

    static const char* defaultFragment = R"(

    #version 330 core
    out vec4 FragColor;

    void main(){
        FragColor = vec4(1.0);
    }

    )";
}

namespace gr::Render{

void init(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;
    glEnable(GL_DEPTH_TEST);
}

void Mesh::upload(const std::vector<float>& vertices){
    vertexCount_ = vertices.size() / 3;
    indexCount_ = 0;

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
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

void Mesh::upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index){
    vertexCount_ = vertices.size() / 3;
    indexCount_ = index.size();

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        index.size() * sizeof(unsigned int),
        index.data(),
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
    if(indexCount_ > 0){
        glDrawElements(drawMode, indexCount_, GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(drawMode, 0, vertexCount_);
    }
    glBindVertexArray(0);
}

Mesh::~Mesh(){
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

Mesh::Mesh(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}

void RenderObject::draw(const Shader& shader) const{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    GLuint mLoc = glGetUniformLocation(shader.getProgram(), "uModel");
    glUniformMatrix4fv(mLoc, 1, GL_FALSE, &model[0][0]);

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