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
#include "granite/core/math.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace gr::Core{

void init(const Config& cfg){
    const int depth   = static_cast<int>(gr::Math::Clamp(static_cast<float>(cfg.depthBits),   16, 32));
    const int stencil = static_cast<int>(gr::Math::Clamp(static_cast<float>(cfg.stencilBits), 0,  8));

    if (glfwInit()) {
        gr::internal::log(
            gr::internal::Severity::INFO,
            gr::internal::Module::WINDOW,
            "Windowing backend (GLFW) initialized."
        );
    } else {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Cannot initialize windowing backend (GLFW)."
        );
    };

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, static_cast<int>(depth));
    glfwWindowHint(GLFW_STENCIL_BITS, static_cast<int>(stencil));

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core initialized."
    );
}

void init() {
    Config cfg = {0};
    cfg.depthBits    = 24;
    cfg.stencilBits  = 8;

    init(cfg);
}

void exit(){
    glfwTerminate();
    
    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::WINDOW,
        "Windowing backend (GLFW) shutdown."
    );

    gr::internal::log(
        gr::internal::Severity::INFO,
        gr::internal::Module::CORE,
        "Core shutdown."
    );
}

}
