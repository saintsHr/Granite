#include "granite/window.hpp"

#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    (void)window;
    glViewport(0, 0, width, height);
}

namespace gr{

Window::Window(const std::string& title, gr::Vec2 size){
    title_ = title;
    size_.x = size.x;
    size_.y = size.y;
};
Window::~Window(){
    size_.x = 0;
    size_.y = 0;
};

void Window::create(){
    raw_ = glfwCreateWindow(size_.x, size_.y, title_.c_str(), NULL, NULL);
    if (raw_ == NULL) return;
    glfwMakeContextCurrent(raw_);
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(raw_, framebuffer_size_callback);
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(raw_);
}

void Window::handle(){
    glfwSwapBuffers(raw_);
    glfwPollEvents(); 
}

void Window::clear(gr::Color3 color){
    glClearColor(
        color.r / 255,
        color.g / 255,
        color.b / 255,
        1.0f
    );
}

void Window::setSize(int width, int height){
    size_.x = width;
    size_.y = height;
}

void Window::setTitle(const std::string& title){
    Window::title_ = title;
}

gr::Vec2 Window::getSize(){
    return size_;
}

std::string Window::getTitle(){
    return title_;
}

}