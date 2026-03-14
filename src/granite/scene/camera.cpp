/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

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

#include "granite/scene/camera.hpp"
#include "granite/core/log.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gr::Scene {

void Camera::update(const gr::Window& window) {
    if (aspect.y <= 0.0f) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::RENDERER,
            "Invalid camera aspect (y)"
        );
    };

    if (aspect.x <= 0.0f) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::RENDERER,
            "Invalid camera aspect (x)"
        );
    };

    aspect = window.getSize();

    glm::mat4 projection = glm::perspective(glm::radians(fov), aspect.x / aspect.y, near, far);

    glm::vec3 front;
    float yawAdjusted = rotation.y - 90.0f;
    front.x = glm::cos(glm::radians(yawAdjusted)) * glm::cos(glm::radians(rotation.x));
    front.y = glm::sin(glm::radians(rotation.x));
    front.z = glm::sin(glm::radians(yawAdjusted)) * glm::cos(glm::radians(rotation.x));

    front = glm::normalize(front);

    glm::vec3 posGLM = {position.x, position.y, position.z};
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

gr::Vec3 Camera::getDirection(gr::Direction direction) {
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    forward.x = static_cast<float>(cos(static_cast<double>(glm::radians(rotation.x))) * sin(static_cast<double>(glm::radians(rotation.y))));
    forward.y = static_cast<float>(sin(static_cast<double>(glm::radians(rotation.x))));
    forward.z = static_cast<float>(-cos(static_cast<double>(glm::radians(rotation.x))) * cos(static_cast<double>(glm::radians(rotation.y))));
    
    forward = glm::normalize(forward);
    right   = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up      = glm::normalize(glm::cross(right, forward));

    switch (direction) {
        case gr::Direction::FRONT: return { forward.x,  forward.y,  forward.z};
        case gr::Direction::BACK:  return {-forward.x, -forward.y, -forward.z};
        case gr::Direction::RIGHT: return { right.x,    right.y,    right.z  };
        case gr::Direction::LEFT:  return {-right.x,   -right.y,   -right.z  };
        case gr::Direction::UP:    return { up.x,       up.y,       up.z     };
        case gr::Direction::DOWN:  return {-up.x,      -up.y,      -up.z     };
        default: break;
    }

    return {0.0f, 0.0f, 0.0f};
}

}