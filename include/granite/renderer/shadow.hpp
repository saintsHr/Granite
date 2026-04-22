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

constexpr GLsizei SHADOW_WIDTH = 2048;
constexpr GLsizei SHADOW_HEIGHT = 2048;
constexpr float SHADOW_DISTANCE = 10.0;
constexpr float SHADOW_NEAR = 0.1f;
constexpr float SHADOW_FAR  = 100.0f;

constexpr float DEPTH_MAP_BORDER_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};

void initShadow();
void shadowPass(const gr::Window* window);

}