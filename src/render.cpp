#include "granite/render.hpp"

#include "GLFW/glfw3.h"

namespace gr{

void Render::init(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;
}

};