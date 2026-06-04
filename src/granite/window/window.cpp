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

#include "granite/window/window.hpp"
#include "granite/core/log.hpp"
#include "glad/glad.h"

#include <GLFW/glfw3.h>

namespace gr{

void Window::framebuffer_size_callback_(GLFWwindow* window, int width, int height) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!self) return;

    glViewport(0, 0, width, height);
    self->size_ = {static_cast<float>(width), static_cast<float>(height)};
}

Window::Window(const std::string& title, gr::Vec2 size) {
    title_ = title;
    size_ = size;

    raw_ = glfwCreateWindow(int(size_.x), int(size_.y), title_.c_str(), NULL, NULL);
    if (raw_ == NULL) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Cannot create window"
        );
    };

    glfwMakeContextCurrent(raw_);

    glfwSetWindowUserPointer(raw_, this);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        gr::internal::log(
            gr::internal::Severity::FATAL,
            gr::internal::Module::WINDOW,
            "Failed to initialize GLAD"
        );
        return;
    }
    
    int fbW, fbH;
    glfwGetFramebufferSize(raw_, &fbW, &fbH);
    size_ = {static_cast<float>(fbW), static_cast<float>(fbH)};

    glViewport(0, 0, int(size_.x), int(size_.y));

    glfwSetFramebufferSizeCallback(raw_, framebuffer_size_callback_);

    glEnable(GL_MULTISAMPLE);
}

Window::~Window() {
    size_ = {0.0f, 0.0f};
    title_ = "";
    raw_ = nullptr;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(raw_);
}

void Window::endFrame() {
    glfwSwapBuffers(raw_);
    glfwPollEvents(); 
}

void Window::close() {
    glfwSetWindowShouldClose(this->getRaw(), GLFW_TRUE);
}

void Window::setSize(int width, int height) {
    glfwSetWindowSize(raw_, width, height);
}

void Window::setTitle(const std::string& title) {
    Window::title_ = title;
    glfwSetWindowTitle(raw_, title.c_str());
}

gr::Vec2 Window::getSize() const {
    return size_;
}

std::string Window::getTitle() const {
    return title_;
}

GLFWwindow* Window::getRaw(){
    return raw_;
}

void Window::setVSync(bool state) {
    glfwMakeContextCurrent(this->getRaw());
    if (state)  glfwSwapInterval(1);
    if (!state) glfwSwapInterval(0);
}

void Window::setMouseLock(bool state) {
    if (state)  glfwSetInputMode(this->getRaw(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!state) glfwSetInputMode(this->getRaw(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

}
