#include "granite/render.hpp"

#include "GLFW/glfw3.h"

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

};