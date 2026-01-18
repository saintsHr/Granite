#include "granite/window.hpp"

#include <GLFW/glfw3.h>
#include <GL/gl.h>

namespace gr{

void Window::framebuffer_size_callback_(GLFWwindow* window, int width, int height){
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!self) return;

    glViewport(0, 0, width, height);
    self->size_ = {(float)width, (float)height};
}

Window::Window(const std::string& title, gr::Vec2 size){
    title_ = title;
    size_ = size;
};
Window::~Window(){
    size_ = {0.0f, 0.0f};
};

void Window::create(){
    raw_ = glfwCreateWindow(size_.x, size_.y, title_.c_str(), NULL, NULL);
    if (raw_ == NULL) return;

    glfwMakeContextCurrent(raw_);

    glfwSetWindowUserPointer(raw_, this);
    
    int fbW, fbH;
    glfwGetFramebufferSize(raw_, &fbW, &fbH);
    size_ = {(float)fbW, (float)fbH};

    glViewport(0, 0, size_.x, size_.y);

    glfwSetFramebufferSizeCallback(raw_, framebuffer_size_callback_);
}

bool Window::shouldClose() const{
    return glfwWindowShouldClose(raw_);
}

void Window::handle(){
    glfwSwapBuffers(raw_);
    glfwPollEvents(); 
}

void Window::clear(gr::Color3 color){
    glClearColor(
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        1.0f
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::setSize(int width, int height){
    glfwSetWindowSize(raw_, width, height);
}

void Window::setTitle(const std::string& title){
    Window::title_ = title;
}

gr::Vec2 Window::getSize() const{
    return size_;
}

std::string Window::getTitle() const{
    return title_;
}

GLFWwindow* Window::getRaw(){
    return raw_;
}

}