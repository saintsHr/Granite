#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstdio>

namespace gr::Render{

void init(){
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) return;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

};