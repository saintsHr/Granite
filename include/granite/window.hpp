#pragma once

#include "granite/vector.hpp"
#include "granite/color.hpp"

#include "GLFW/glfw3.h"
#include <string>

namespace gr{

class Window{
public:

    Window(const std::string& title, gr::Vec2 size);
    ~Window();
    
    void setSize(int width, int height);
    void setTitle(const std::string& title);
    gr::Vec2 getSize();
    std::string getTitle();
    void create();
    bool shouldClose();
    void handle();
    void clear(gr::Color3 color);

private:

    gr::Vec2 size_;
    std::string title_;
    GLFWwindow* raw_;

};

}