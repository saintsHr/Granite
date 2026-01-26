#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "granite/render/render.hpp"

namespace gr::Render{

FrameContext gFrame;

void init(){
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void beginFrame(const gr::Scene::Camera& camera){
    gFrame.view = camera.getView();
    gFrame.projection = camera.getProjection();
}

};