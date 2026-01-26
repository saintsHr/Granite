#include "granite/render/material.hpp"

namespace gr::Render{

void Material::bind(){
    shader->use();

    if (cL_ == -1){
        cL_ = glGetUniformLocation(shader->getProgram(), "uColor");
    }
    GLint cLoc = cL_;

    if (cLoc != -1){
        glUniform3f(
            cLoc,
            static_cast<float>(color.r) * (1.0f / 255.0f),
            static_cast<float>(color.g) * (1.0f / 255.0f),
            static_cast<float>(color.b) * (1.0f / 255.0f)
        );
    }
}

}