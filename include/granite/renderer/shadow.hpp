#pragma once

#include "glad/glad.h"
#include "granite/window/window.hpp"
#include "granite/renderer/shader.hpp"

#include <memory>

namespace gr::internal {

extern const char* defaultVertexShadowShader;
extern const char* defaultFragmentShadowShader;

}

namespace gr::Renderer {

extern std::unique_ptr<gr::Renderer::Shader> shadowShader;
extern GLuint depthMap;
extern GLuint depthMapFBO;

constexpr GLsizei SHADOW_RESOLUTION = 1024 * 8;

constexpr float DEPTH_MAP_BORDER_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};

void initShadow();
void shadowPass(const gr::Window* window);

}