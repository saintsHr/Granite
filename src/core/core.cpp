#include "granite/core/core.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr{
    
void Core::init(){
    if (!glfwInit()) return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void Core::exit(){
    glfwTerminate();
}

}