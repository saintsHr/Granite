#pragma once

#include "granite/render/shader.hpp"
#include "granite/window/window.hpp"

namespace gr::Scene {

class Camera {
public:
    gr::Vec3 pos = {0.0f, 0.0f, 0.0f};
    gr::Vec2 rotation = {0.0f, 0.0f};
    gr::Vec2 aspect = {1.0f, 1.0f};
    float fov = 75.0f;
    float near = 0.1f;
    float far = 100.0f;

    glm::mat4 getProjection() const;
    glm::mat4 getView() const;

    void update(const gr::Window& window);
    void moveForward(float speed);
    void moveBack(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveDown(float speed);
    void moveUp(float speed);
private:
    glm::mat4 projection_;
    glm::mat4 view_;
};

}