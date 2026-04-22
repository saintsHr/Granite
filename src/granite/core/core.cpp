/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "granite/core/core.hpp"
#include "granite/core/log.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr::Core{

void init(const Config& cfg){
    // converts & clamps cfg info
    float depth, stencil, samples;
    depth   = gr::Math::clamp(static_cast<float>(cfg.depthBits),          16.0f,    32.0f);
    stencil = gr::Math::clamp(static_cast<float>(cfg.stencilBits),        0.0f,     8.0f);
    samples = gr::Math::clamp(static_cast<float>(cfg.msaaSamples),        0.0f,     8.0f);

    // tryes to init GLFW
    if (!glfwInit()) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Cannot initialize windowing backend (GLFW)."
        );
    } else {
        gr::internal::log(
            gr::internal::Severity::INFO,
            gr::internal::Module::WINDOW,
            "Windowing backend (GLFW) initialized."
        );
    };

    // GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, static_cast<int>(depth));
    glfwWindowHint(GLFW_STENCIL_BITS, static_cast<int>(stencil));
    glfwWindowHint(GLFW_SAMPLES, static_cast<int>(samples));

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core initialized."
    );
}

void init(){
    // tryes to init GLFW
    if (!glfwInit()) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Cannot initialize windowing backend (GLFW)."
        );
    };

    // GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 4);

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core initialized."
    );
}

void exit(){
    glfwTerminate();
    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Windowing backend (GLFW) shutdown."
    );

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core shutdown."
    );
}

}