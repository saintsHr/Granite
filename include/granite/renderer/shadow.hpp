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

extern GLuint depthMap;
extern GLuint depthMapFBO;

extern const GLsizei SHADOW_WIDTH;
extern const GLsizei SHADOW_HEIGHT;

extern std::unique_ptr<gr::Renderer::Shader> shadowShader;

void initShadow();
void shadowPass(const gr::Window* window);

}