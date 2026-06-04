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

#include "granite/core/math.hpp"
#include "granite/renderer/renderer.hpp"
#include "granite/renderer/material.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

namespace gr::Renderer{

void Material::bind(){
    shader->use();

    if (cL_  == -1) cL_  = glGetUniformLocation(shader->getProgram(), "uColor");
    if (tL_  == -1) tL_  = glGetUniformLocation(shader->getProgram(), "uTexture");
    if (sL_  == -1) sL_  = glGetUniformLocation(shader->getProgram(), "uShininess");
    if (oL_  == -1) oL_  = glGetUniformLocation(shader->getProgram(), "uOpacity");
    if (hL_  == -1) hL_  = glGetUniformLocation(shader->getProgram(), "uHasTexture");
    if (scL_ == -1) scL_ = glGetUniformLocation(shader->getProgram(), "uSpecularColor");

    if (cL_ != -1){
        glUniform3f(
            cL_,
            gr::Math::Normalize(static_cast<float>(color.r), 0.0f, 255.0f),
            gr::Math::Normalize(static_cast<float>(color.g), 0.0f, 255.0f),
            gr::Math::Normalize(static_cast<float>(color.b), 0.0f, 255.0f)
        );
    }

    if (scL_ != -1){
        glUniform3f(
            scL_,
            gr::Math::Normalize(static_cast<float>(specularColor.r), 0.0f, 255.0f),
            gr::Math::Normalize(static_cast<float>(specularColor.g), 0.0f, 255.0f),
            gr::Math::Normalize(static_cast<float>(specularColor.b), 0.0f, 255.0f)
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

    if (tL_ != -1 && texture.getID_() != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getID_());
        glUniform1i(tL_, 0);
    }

    if (hL_ != -1) {
        glUniform1i(
            hL_,
            texture.getID_() != 0 ? true : false
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

Material::Material() {
    shader = std::make_shared<Shader>();
}

}
