#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr::Render{

void init(){
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return;
    glEnable(GL_DEPTH_TEST);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
}

};