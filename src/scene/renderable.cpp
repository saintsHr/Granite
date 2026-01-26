#include "granite/scene/renderable.hpp"
#include "granite/render/render.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace gr::Scene{

void RenderObject::draw(GLenum drawMode){
    if (!mesh || !material.shader) return;
    material.bind();

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
    if (vLoc != -1) glUniformMatrix4fv(vLoc, 1, GL_FALSE, &gr::Render::gFrame.view[0][0]);
    if (pLoc != -1) glUniformMatrix4fv(pLoc, 1, GL_FALSE, &gr::Render::gFrame.projection[0][0]);

    // draws mesh
    mesh->draw(*material.shader, drawMode);
}

}