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

#pragma once

#include <string>

namespace gr::Renderer {

class Shader;
extern const Shader* currentShader;

class Shader {
public:
    Shader();
    Shader(const std::string& vertexStr, const std::string& fragmentStr);
    ~Shader();

    void use() const;

    void setMat4(const std::string &name, const float* mat) const;
    void setInt1(const std::string &name, int n) const;
    void setFloat1(const std::string &name, float n) const;

    unsigned int getProgram() const;
    int getPLoc() const;
    int getVLoc() const;
    void setPLoc(int loc);
    void setVLoc(int loc);
private:
    std::string load_(const std::string& filename) const;

    unsigned int program_;
    int pLoc_ = -1;
    int vLoc_ = -1;
};

}
