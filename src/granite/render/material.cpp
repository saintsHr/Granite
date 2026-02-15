#include "granite/render/render.hpp"
#include "granite/render/material.hpp"

namespace gr::Render{

void Material::bind(){
    shader->use();

    if (cL_ == -1) cL_ = glGetUniformLocation(shader->getProgram(), "uColor");
    if (sL_ == -1) sL_ = glGetUniformLocation(shader->getProgram(), "uShininess");
    if (oL_ == -1) oL_ = glGetUniformLocation(shader->getProgram(), "uOpacity");

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
            shininess
        );
    }

    if (oL_ != -1){
        glUniform1f(
            oL_,
            opacity
        );
    }

    if (opacity < 1.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    } else {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}

}