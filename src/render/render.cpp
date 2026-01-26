#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "granite/render/render.hpp"

namespace gr::Render{

void init(){
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void beginFrame(const gr::Scene::Camera& camera, Shader& shader){
    GLint pLoc = shader.getPLoc();
    GLint vLoc = shader.getVLoc();

    if (pLoc == -1){
        pLoc = glGetUniformLocation(shader.getProgram(), "uProjection");
        shader.setPLoc(pLoc);
    }
    if (vLoc == -1){
        vLoc = glGetUniformLocation(shader.getProgram(), "uView");
        shader.setVLoc(vLoc);
    }

    glUniformMatrix4fv(pLoc, 1, GL_FALSE, &camera.getProjection()[0][0]);
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, &camera.getView()[0][0]);
}

};