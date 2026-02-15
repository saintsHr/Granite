#pragma once

#include "granite/render/mesh.hpp"
#include "granite/render/shader.hpp"
#include "granite/scene/camera.hpp"
#include "granite/scene/renderable.hpp"

namespace gr::Render {

struct FrameContext {
    glm::mat4 view;
    glm::mat4 projection;
    gr::Vec3 cameraPos;
};

extern FrameContext gFrame;
    
void init();
void beginFrame(const gr::Scene::Camera& camera);

void addToQueue(const gr::Scene::RenderObject& obj);

void endFrame();

}