#include "granite/scene/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gr::Scene {

void Camera::update(const gr::Window& window) {
    if (aspect.y <= 0.0f) return;
    if (aspect.x <= 0.0f) return;
    aspect = window.getSize();

    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect.x / aspect.y, near, far);

    glm::vec3 front;
    float yawAdjusted = rotation.y - 90.0f;
    front.x = float(cos(glm::radians(yawAdjusted)) * cos(glm::radians(rotation.x)));
    front.y = float(sin(glm::radians(rotation.x)));
    front.z = float(sin(glm::radians(yawAdjusted)) * cos(glm::radians(rotation.x)));
    front = glm::normalize(front);

    glm::vec3 posGLM = {pos.x, pos.y, pos.z};
    glm::vec3 cameraTarget = posGLM + front;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(posGLM, cameraTarget, up);

    projection_ = projection;
    view_ = view;
}

glm::mat4 Camera::getProjection() const {
    return projection_;
}

glm::mat4 Camera::getView() const {
    return view_;
}

void Camera::moveForward(float speed) {
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = float(cos(yawRad));
    forward.y = 0.0f;
    forward.z = float(sin(yawRad));
    forward = glm::normalize(forward);

    pos += forward * speed;
}

void Camera::moveBack(float speed) {
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = float(cos(yawRad));
    forward.y = 0.0f;
    forward.z = float(sin(yawRad));
    forward = glm::normalize(forward);

    pos -= forward * speed;
}

void Camera::moveLeft(float speed) {
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = float(cos(yawRad));
    forward.y = 0.0f;
    forward.z = float(sin(yawRad));
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(
        glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))
    );

    pos -= right * speed;
}

void Camera::moveRight(float speed) {
    float yawRad = glm::radians(rotation.y - 90.0f);

    glm::vec3 forward;
    forward.x = float(cos(yawRad));
    forward.y = 0.0f;
    forward.z = float(sin(yawRad));
    forward = glm::normalize(forward);

    glm::vec3 right = glm::normalize(
        glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))
    );

    pos += right * speed;
}

void Camera::moveDown(float speed) {
    this->pos.y -= speed;
}

void Camera::moveUp(float speed) {
    this->pos.y += speed;
}

}