#pragma once

#include "granite/core/vector.hpp"
#include "granite/core/color.hpp"

#include "GLFW/glfw3.h"
#include <string>

namespace gr {

class Window {
public:
    Window(const std::string& title, gr::Vec2 size);
    ~Window();
    
    void setSize(int width, int height);
    void setTitle(const std::string& title);

    gr::Vec2 getSize() const;
    std::string getTitle() const;

    void setVSync(bool state);
    void setMouseLock(bool state);

    void close();

    bool shouldClose() const;
    void endFrame();

    GLFWwindow* getRaw();
private:
    gr::Vec2 size_;
    std::string title_;
    GLFWwindow* raw_;
    static void framebuffer_size_callback_(GLFWwindow* window, int width, int height);
};

}