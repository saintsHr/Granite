#include "granite/scene/renderable.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace gr::Scene{

void RenderObject::draw(const gr::Render::Shader& shader, GLenum drawMode) const{
    shader.use();

    // calculates transform
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    // checks if model location exists, if not, find it
    GLint mLoc;
    if (mL_ == -1){
        mLoc = glGetUniformLocation(shader.getProgram(), "uModel");
    } else {
        mLoc = mL_;
    }

    // checks if color location exists, if not, find it
    GLint cLoc;
    if (cL_ == -1){
        cLoc = glGetUniformLocation(shader.getProgram(), "uColor");
    } else {
        cLoc = cL_;
    }

    // applies transform if valid location
    if (mLoc != -1){
        glUniformMatrix4fv(
            mLoc,
            1,
            GL_FALSE,
            &model[0][0]
        );
    }

    // applies color if valid location
    if (cLoc != -1){
        glUniform3f(
            cLoc,
            static_cast<float>(material.color.r) * (1.0f / 255.0f),
            static_cast<float>(material.color.g) * (1.0f / 255.0f),
            static_cast<float>(material.color.b) * (1.0f / 255.0f)
        );
    }

    // draws mesh
    mesh->draw(shader, drawMode);
}

}