#include "granite/render/render.hpp"
#include "granite/render/material.hpp"

namespace gr::Render{

void Material::bind(){
    shader->use();

    if (cL_ == -1) cL_ = glGetUniformLocation(shader->getProgram(), "uColor");
    if (tL_ == -1) tL_ = glGetUniformLocation(shader->getProgram(), "uTexture");
    if (sL_ == -1) sL_ = glGetUniformLocation(shader->getProgram(), "uShininess");
    if (oL_ == -1) oL_ = glGetUniformLocation(shader->getProgram(), "uOpacity");
    if (hL_ == -1) hL_ = glGetUniformLocation(shader->getProgram(), "uHasTexture");

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

    if (tL_ != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id());
        glUniform1i(glGetUniformLocation(shader->getProgram(), "uTexture"), 0);
    }

    if (texture.id() != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.id());
        glUniform1i(tL_, 0);
    }

    if (hL_ != -1) {
        glUniform1i(
            hL_,
            texture.id() != 0 ? 1 : 0
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