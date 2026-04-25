#include "granite/renderer/shadow.hpp"
#include "granite/renderer/renderer.hpp"
#include "granite/scene/light.hpp"

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/vector_relational.hpp>
#include <glm/ext.hpp>

namespace gr::internal {

const char* defaultVertexShadowShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uLightSpace;

void main() {
    gl_Position = uLightSpace * uModel * vec4(aPos, 1.0);
}

// ------------------------------------------------------------------------------//

)glsl";

const char* defaultFragmentShadowShader = R"glsl(

// ------------------------------------------------------------------------------//

#version 330 core
void main() {}

// ------------------------------------------------------------------------------//

)glsl";

}

namespace gr::Renderer {

constexpr float DEPTH_MAP_BORDER_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};
std::unique_ptr<gr::Renderer::Shader> shadowShader;
GLuint depthMap    = 0;
GLuint depthMapFBO = 0;
GLint  lsL_ = -1;
GLint  mL_  = -1;

void initShadow() {
    // --- generates depth map & depth map FBO ---
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);

    // --- creates depth map texture ---
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT, 
        8192,
        8192,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameterfv(
        GL_TEXTURE_2D,
        GL_TEXTURE_BORDER_COLOR,
        DEPTH_MAP_BORDER_COLOR
    );

    // --- attach depth map texture to depth map FBO ---
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depthMap,
        0
    );
    
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // --- creates shadow shader ---
    shadowShader = std::make_unique<gr::Renderer::Shader>(
        gr::internal::defaultVertexShadowShader,
        gr::internal::defaultFragmentShadowShader
    );
}

static std::vector<glm::vec3> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) {
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec3> corners;
    corners.reserve(8);

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 2; ++y) {
            for (int z = 0; z < 2; ++z) {
                glm::vec4 pt = inv * glm::vec4(
                    2.0f * static_cast<float>(x) - 1.0f,
                    2.0f * static_cast<float>(y) - 1.0f,
                    2.0f * static_cast<float>(z) - 1.0f,
                    1.0f
                );

                pt /= pt.w;
                corners.push_back(glm::vec3(pt));
            }
        }
    }

    return corners;
}

static void calcLightSpace() {
    auto& dirLights = gr::Scene::LightManager::getDirectionalLights();
    if (dirLights.empty()) return;

    gFrame.lightSpaces.resize(dirLights.size());

    glm::mat4 proj = gFrame.projection;
    glm::mat4 view = gFrame.view;

    auto corners = getFrustumCornersWorldSpace(proj, view);

    int i = 0;
    for (auto& [id, light] : dirLights) {
        glm::vec3 lightDir = glm::normalize(glm::vec3(
            light.direction.x,
            light.direction.y,
            light.direction.z
        ));

        glm::vec3 center(0.0f);
        for (const auto& v : corners) center += v;
        center /= corners.size();

        glm::vec3 up = abs(lightDir.y) > 0.9f
            ? glm::vec3(0, 0, 1)
            : glm::vec3(0, 1, 0);

        float radius = 0.0f;
        for (const auto& corner : corners) {
            radius = std::max(radius, glm::length(corner - center));
        }

        glm::mat4 lightView = glm::lookAt(
            center - lightDir * radius,
            center,
            up
        );

        float minX =  FLT_MAX;
        float maxX = -FLT_MAX;
        float minY =  FLT_MAX;
        float maxY = -FLT_MAX;
        float minZ =  FLT_MAX;
        float maxZ = -FLT_MAX;

        for (const auto& corner : corners) {
            glm::vec4 trf = lightView * glm::vec4(corner, 1.0f);

            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }

        float zExtent = maxZ - minZ;
        float zPadding = zExtent * 0.05f;

        minZ -= zPadding;
        maxZ += zPadding;

        if (minZ > maxZ) std::swap(minZ, maxZ);

        float nearPlane = -maxZ;
        float farPlane  = -minZ;

        glm::mat4 lightProj = glm::ortho(
            minX, maxX,
            minY, maxY,
            nearPlane,
            farPlane
        );

        gFrame.lightSpaces[static_cast<unsigned long>(i++)] = lightProj * lightView;
    }
}

void shadowPass(const gr::Window* window) {
    calcLightSpace();

    glCullFace(GL_FRONT);

    glViewport(0, 0, 8192, 8192);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowShader->use();

    if (lsL_ == -1) {
        lsL_ = glGetUniformLocation(
            shadowShader->getProgram(),
            "uLightSpace"
        );
    }

    glUniformMatrix4fv(
        lsL_,
        1,
        GL_FALSE,
        glm::value_ptr(gFrame.lightSpaces[0])
    );

    for (size_t i = 0; i < opaqueObjects.size(); i++) {
        if (mL_ == -1) {
            mL_ = glGetUniformLocation(
                shadowShader->getProgram(),
                "uModel"
            );
        }

        glUniformMatrix4fv(
            mL_,
            1,
            GL_FALSE,
            glm::value_ptr(opaqueObjects[i].transform.getMatrix())
        );

        opaqueObjects[i].draw();
    }

    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(
        0,
        0,
        static_cast<GLsizei>(window->getSize().x),
        static_cast<GLsizei>(window->getSize().y)
    );
}

}