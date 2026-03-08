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