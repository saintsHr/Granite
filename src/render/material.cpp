#include "granite/render/render.hpp"
#include "granite/render/material.hpp"

namespace gr::Render{

void Material::bind(){
    shader->use();

    GLint vLoc = glGetUniformLocation(shader->getProgram(), "uView");
    GLint pLoc = glGetUniformLocation(shader->getProgram(), "uProjection");

    if (vLoc != -1) glUniformMatrix4fv(vLoc, 1, GL_FALSE, &gr::Render::gFrame.view[0][0]);
    if (pLoc != -1) glUniformMatrix4fv(pLoc, 1, GL_FALSE, &gr::Render::gFrame.projection[0][0]);

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