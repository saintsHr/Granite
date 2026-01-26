#pragma once

#include "granite/render/mesh.hpp"
#include "granite/render/shader.hpp"
#include "granite/scene/camera.hpp"

namespace gr::Render{
    
void init();
void beginFrame(const gr::Scene::Camera& camera, Shader& shader);

};