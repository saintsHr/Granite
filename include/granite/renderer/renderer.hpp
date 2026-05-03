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

#pragma once

#include "granite/renderer/mesh.hpp"
#include "granite/renderer/shader.hpp"
#include "granite/scene/camera.hpp"
#include "granite/renderer/renderable.hpp"

namespace gr::Renderer {

struct FrameContext {
    glm::mat4 view;
    glm::mat4 projection;
    gr::Vec3 cameraPos;

    std::vector<glm::mat4> lightSpaces;
};

extern FrameContext gFrame;
extern unsigned int depthMap;
extern bool gShadowPass;
extern std::vector<gr::Renderer::RenderObject> opaqueObjects;
extern std::vector<gr::Renderer::RenderObject> transparentObjects;

void init();
void beginFrame(const gr::Scene::Camera& camera);
void addToQueue(const gr::Renderer::RenderObject& obj);
void endFrame();

}
