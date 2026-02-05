#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "granite/render/render.hpp"
#include "granite/scene/light.hpp"

namespace gr::Render{

FrameContext gFrame;

GLuint lightUBO;

void init(){
    // loads GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // creates light UBO
    glGenBuffers(1, &lightUBO);

    // binds light UBO
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
    glBufferData(
        GL_UNIFORM_BUFFER,
        sizeof(gr::Scene::GPULightBlock),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    // binds light UBO
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, lightUBO);
}

void beginFrame(const gr::Scene::Camera& camera){
    gFrame.view = camera.getView();
    gFrame.projection = camera.getProjection();

    gr::Scene::GPULightBlock block{};
    block.counts = glm::ivec4(0);

    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(gr::Scene::GPULightBlock), &block);
}

};