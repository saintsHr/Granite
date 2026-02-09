#pragma once

#include "glad/glad.h"

#include <string>

namespace gr::Render {

class Shader;
const extern Shader* currentShader;

class Shader {
public:
    Shader();
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    ~Shader();
    void use() const;

    GLuint getProgram() const;
    GLint getPLoc() const;
    GLint getVLoc() const;
    void setPLoc(GLint loc);
    void setVLoc(GLint loc);
private:
    std::string load_(const std::string& filename) const;

    GLuint program_;
    GLint pLoc_ = -1;
    GLint vLoc_ = -1;
};

}