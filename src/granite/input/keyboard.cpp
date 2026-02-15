#include <GLFW/glfw3.h>

#include "granite/input/input.hpp"

namespace gr::Input{

bool KeyPressed(gr::Window window, Input::Key key){
    if (glfwGetKey(window.getRaw(), static_cast<int>(key)) == GLFW_PRESS){
        return true;
    } else {
        return false;
    }
}

}