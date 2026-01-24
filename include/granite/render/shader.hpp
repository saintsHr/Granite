#pragma once

#include "glad/glad.h"

namespace gr::Render{

class Shader{
public:
    Shader();
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();
    void use() const;
    GLuint getProgram() const;
private:
    GLuint program_;
};

}