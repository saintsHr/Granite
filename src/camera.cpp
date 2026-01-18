#include "granite/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gr{

void Camera::update(const gr::Render::Shader& shader, const gr::Window& window){
    if (aspect.y <= 0.0f) return;
    if (aspect.x <= 0.0f) return;
    aspect = window.getSize();

    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect.x / aspect.y, near, far);

    glm::vec3 front;
    float yawAdjusted = rotation.y - 90.0f;
    front.x = cos(glm::radians(yawAdjusted)) * cos(glm::radians(rotation.x));
    front.y = sin(glm::radians(rotation.x));
    front.z = sin(glm::radians(yawAdjusted)) * cos(glm::radians(rotation.x));
    front = glm::normalize(front);

    glm::vec3 posGLM = {pos.x, pos.y, pos.z};
    glm::vec3 cameraTarget = posGLM + front;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(posGLM, cameraTarget, up);

    GLuint pLoc = glGetUniformLocation(shader.getProgram(), "uProjection");
    GLuint vLoc = glGetUniformLocation(shader.getProgram(), "uView");
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, &view[0][0]);
}

void Camera::moveForward(float speed){
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = cos(yawRad);
    forward.y = 0.0f;
    forward.z = sin(yawRad);
    forward = glm::normalize(forward);

    pos += forward * speed;
}

void Camera::moveBack(float speed){
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = cos(yawRad);
    forward.y = 0.0f;
    forward.z = sin(yawRad);
    forward = glm::normalize(forward);

    pos -= forward * speed;
}

void Camera::moveLeft(float speed){
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = cos(yawRad);
    forward.y = 0.0f;
    forward.z = sin(yawRad);
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(
        glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))
    );

    pos -= right * speed;
}

void Camera::moveRight(float speed){
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = cos(yawRad);
    forward.y = 0.0f;
    forward.z = sin(yawRad);
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(
        glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))
    );

    pos += right * speed;
}

}