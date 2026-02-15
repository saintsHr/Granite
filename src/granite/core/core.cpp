#include "granite/core/core.hpp"
#include "granite/core/log.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr::Core{

void init(const Config& cfg){
    // converts & clamps cfg info
    float depth, stencil, samples;
    depth   = gr::Math::Clamp(static_cast<float>(cfg.depthBits),   16.0f, 32.0f);
    stencil = gr::Math::Clamp(static_cast<float>(cfg.stencilBits),  0.0f,  8.0f);
    samples = gr::Math::Clamp(static_cast<float>(cfg.msaaSamples),  0.0f,  8.0f);

    // tryes to init GLFW
    if (!glfwInit()) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Cannot initialize windowing backend (GLFW)"
        );
    };

    // GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, static_cast<int>(depth));
    glfwWindowHint(GLFW_STENCIL_BITS, static_cast<int>(stencil));
    glfwWindowHint(GLFW_SAMPLES, static_cast<int>(samples));

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core initialized"
    );
}

void init(){
    // tryes to init GLFW
    if (!glfwInit()) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Cannot initialize windowing backend (GLFW)"
        );
    };

    // GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 4);

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core initialized"
    );
}

void exit(){
    glfwTerminate();
}

}