#include "granite/core/core.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr::Core{

void init(const Config& cfg){
    unsigned int depth, stencil, samples;
    depth   = static_cast<unsigned int>(gr::Math::Clamp(cfg.depthBits,   16, 32));
    stencil = static_cast<unsigned int>(gr::Math::Clamp(cfg.stencilBits,  0,  8));
    samples = static_cast<unsigned int>(gr::Math::Clamp(cfg.msaaSamples,  0,  8));

    if (!glfwInit()) return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, depth);
    glfwWindowHint(GLFW_STENCIL_BITS, stencil);
    glfwWindowHint(GLFW_SAMPLES, samples);

    gr::Time::GetElapsedTimeMS();
}

void init(){
    if (!glfwInit()) return;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 4);

    gr::Time::GetElapsedTimeMS();
}

void exit(){
    glfwTerminate();
}

}