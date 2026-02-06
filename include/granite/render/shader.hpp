#pragma once

#include "glad/glad.h"

namespace gr::Render {

class Shader {
public:
    Shader();
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();
    void use() const;

    GLuint getProgram() const;
    GLint getPLoc() const;
    GLint getVLoc() const;
    void setPLoc(GLint loc);
    void setVLoc(GLint loc);
private:
    GLuint program_;
    GLint pLoc_ = -1;
    GLint vLoc_ = -1;
};

}