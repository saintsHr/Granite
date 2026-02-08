#include "granite/render/render.hpp"
#include "granite/render/material.hpp"

namespace gr::Render{

void Material::bind(){
    shader->use();

    if (cL_ == -1) cL_ = glGetUniformLocation(shader->getProgram(), "uColor");
    if (sL_ == -1) sL_ = glGetUniformLocation(shader->getProgram(), "uShininess");

    if (cL_ != -1){
        glUniform3f(
            cL_,
            static_cast<float>(color.r) * (1.0f / 255.0f),
            static_cast<float>(color.g) * (1.0f / 255.0f),
            static_cast<float>(color.b) * (1.0f / 255.0f)
        );
    }

    if (sL_ != -1){
        glUniform1f(
            sL_,
            static_cast<float>(shininess)
        );
    }
}

}