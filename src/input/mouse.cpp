#include "granite/input/mouse.hpp"

namespace gr::Input{

void MouseLocked(gr::Window window, bool state){
    if (state)  glfwSetInputMode(window.getRaw(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!state) glfwSetInputMode(window.getRaw(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

gr::Vec2 GetMousePosition(gr::Window window){
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

gr::Vec2 GetMouseDelta(gr::Window window){
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