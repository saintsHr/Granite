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

#include "granite/renderer/renderable.hpp"
#include "granite/core/log.hpp"
#include "granite/internal.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"

namespace gr::Renderer{

void RenderObject::draw() {
    if (!mesh) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::RENDERER,
            "Mesh not found in RenderObject"
        );

        return;
    };

    if (!material.shader) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::RENDERER,
            "Material.shader not found in RenderObject"
        );

        return;
    }

    material.bind();

    // updates uCameraPos
    if (cmL_ == -1) cmL_ = glGetUniformLocation(material.shader->getProgram(), "uCameraPos");
    if (cmL_ != -1) {
        glUniform3f(
            cmL_,
            gr::Renderer::gFrame.cameraPos.x,
            gr::Renderer::gFrame.cameraPos.y,
            gr::Renderer::gFrame.cameraPos.z
        );
    }

    // calculates transform
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    // checks if shaders locations exists, if not, finds it
    if (mL_ == -1) mL_ = glGetUniformLocation(material.shader->getProgram(), "uModel");
    GLint mLoc = mL_;

    if (vL_ == -1) vL_ = glGetUniformLocation(material.shader->getProgram(), "uView");
    GLint vLoc = vL_;

    if (pL_ == -1) pL_ = glGetUniformLocation(material.shader->getProgram(), "uProjection");
    GLint pLoc = pL_;

    // applies transform, projection and view if valid location
    if (mLoc != -1) glUniformMatrix4fv(mLoc, 1, GL_FALSE, &model[0][0]);
    if (vLoc != -1) glUniformMatrix4fv(vLoc, 1, GL_FALSE, &gr::Renderer::gFrame.view[0][0]);
    if (pLoc != -1) glUniformMatrix4fv(pLoc, 1, GL_FALSE, &gr::Renderer::gFrame.projection[0][0]);

    // draws mesh
    mesh->draw(*material.shader);

    if (material.opacity < 1.0f) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
}

}
