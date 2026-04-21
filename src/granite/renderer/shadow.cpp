#include "granite/renderer/shadow.hpp"
#include "granite/renderer/renderer.hpp"
#include "granite/scene/light.hpp"

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

std::unique_ptr<gr::Renderer::Shader> shadowShader;
GLuint depthMap    = 0;
GLuint depthMapFBO = 0;

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
        SHADOW_WIDTH,
        SHADOW_HEIGHT,
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

void calcLightSpace() {
    glm::vec3 camPos = glm::vec3(
        gFrame.cameraPos.x,
        gFrame.cameraPos.y,
        gFrame.cameraPos.z
    );
    glm::vec3 camForward = -glm::vec3(gFrame.view[2]);
    glm::vec3 frustumCenter = camPos + camForward * SHADOW_DISTANCE;

    auto& dirLights = gr::Scene::LightManager::getDirectionalLights();
    if (!dirLights.empty()) {
        gFrame.lightSpaces.resize(dirLights.size());

        int i = 0;
        for (auto& [id, light] : dirLights) {
            glm::vec3 lightDir = glm::normalize(glm::vec3(
                light.direction.x,
                light.direction.y,
                light.direction.z
            ));

            glm::vec3 lightPos = frustumCenter - lightDir * SHADOW_DISTANCE;

            glm::mat4 lightView = glm::lookAt(
                lightPos,
                frustumCenter,
                glm::vec3(0,1,0)
            );

            glm::mat4 lightProj = glm::ortho(
                -SHADOW_DISTANCE, SHADOW_DISTANCE,
                -SHADOW_DISTANCE, SHADOW_DISTANCE,
                0.1f, 100.0f
            );

            gFrame.lightSpaces[static_cast<unsigned long>(i++)] = lightProj * lightView;
        }
    }
}

void shadowPass(const gr::Window* window) {
    calcLightSpace();

    glDisable(GL_CULL_FACE);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowShader->use();

    GLint lightSpaceLoc = glGetUniformLocation(
        shadowShader->getProgram(),
        "uLightSpace"
    );

    glUniformMatrix4fv(
        lightSpaceLoc,
        1,
        GL_FALSE,
        glm::value_ptr(gFrame.lightSpaces[0])
    );

    for (size_t i = 0; i < opaqueObjects.size(); i++) {
        GLint modelLoc = glGetUniformLocation(
            shadowShader->getProgram(),
            "uModel"
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(opaqueObjects[i].transform.getMatrix())
        );

        opaqueObjects[i].draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window->getSize().x, window->getSize().y);
}

}