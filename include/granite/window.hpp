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
    gr::Vec2 getSize() const;
    std::string getTitle() const;
    void create();
    bool shouldClose() const;
    void handle();
    void clear(gr::Color3 color);
    GLFWwindow* getRaw();

private:

    gr::Vec2 size_;
    std::string title_;
    GLFWwindow* raw_;
    static void framebuffer_size_callback_(GLFWwindow* window, int width, int height);

};

}