#pragma once

#include "granite/render/mesh.hpp"
#include "granite/render/shader.hpp"
#include "granite/scene/camera.hpp"

namespace gr::Render{

struct FrameContext{
    glm::mat4 view;
    glm::mat4 projection;
};

extern FrameContext gFrame;
    
void init();
void beginFrame(const gr::Scene::Camera& camera);

};