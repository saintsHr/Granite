/*
MIT License

Copyright (c) 2026 saintsHr

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

#include "granite/input/mouse.hpp"
#include <GLFW/glfw3.h>

namespace gr::Input{

gr::Vec2 getMousePosition(gr::Window window){
    double x, y;
    glfwGetCursorPos(window.getRaw(), &x, &y);
    return {static_cast<float>(x), static_cast<float>(y)};
}

static double s_lastX   = 0.0;
static double s_lastY   = 0.0;
static double s_accDX   = 0.0;
static double s_accDY   = 0.0;
static bool   s_hasLast = false;
static bool   s_init    = false;
static GLFWwindow* s_window = nullptr;

static void _mouseCallback(GLFWwindow*, double x, double y) {
    if (!s_hasLast) {
        s_lastX = x;
        s_lastY = y;
        s_hasLast = true;
        return;
    }

    s_accDX += x - s_lastX;
    s_accDY += s_lastY - y;

    s_lastX = x;
    s_lastY = y;
}

gr::Vec2 getMouseDelta(gr::Window window){
    if (!s_init || s_window != window.getRaw()){
        s_window = window.getRaw();
        glfwSetCursorPosCallback(window.getRaw(), _mouseCallback);

        s_hasLast = false;
        s_accDX = 0.0;
        s_accDY = 0.0;

        s_init = true;
    }

    gr::Vec2 out {
        static_cast<float>(s_accDX),
        static_cast<float>(s_accDY)
    };

    s_accDX = 0.0;
    s_accDY = 0.0;

    return out;
}

}
