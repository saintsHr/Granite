#pragma once

#include "granite/render.hpp"
#include "granite/window.hpp"

namespace gr{

class Camera{
public:
    gr::Vec3 pos = {0.0f, 0.0f, 0.0f};
    gr::Vec2 rotation = {0.0f, 0.0f};
    gr::Vec2 aspect = {1.0f, 1.0f};
    float fov = 75.0f;
    float near = 0.1f;
    float far = 100.0f;

    void update(const gr::Render::Shader& shader, const gr::Window& window);
    void moveForward(float speed);
    void moveBack(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
};

}