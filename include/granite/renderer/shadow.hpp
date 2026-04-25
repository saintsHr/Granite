#pragma once

#include "granite/window/window.hpp"
#include "granite/renderer/shader.hpp"

#include <memory>

namespace gr::Renderer {

extern std::unique_ptr<gr::Renderer::Shader> shadowShader;

void initShadow();
void shadowPass(const gr::Window* window);

}