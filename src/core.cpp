#include "granite/core.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr{
    
void Core::init(){
    // inits GLFW with hints
    if (!glfwInit()) return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // inits GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;
}
void Core::exit(){
    glfwTerminate();
}

}