#include "VolumetricFog.hpp"
#include <GL/glew.h>
#include <iostream>
#include "../math/Mat4.hpp"
#include "../utils/Time.hpp"
#include "../camera/Camera.hpp"
#include "../light/ShadowMap.hpp"
#include "../light/SceneLightInfo.hpp"

VolumetricFog::VolumetricFog() : FBO(0), framebufferTexture(0), depthTexture(0), framebufferProgram(0), rectVAO(0), rectVBO(0)
{
}

VolumetricFog::~VolumetricFog()
{
    Release();
}

void VolumetricFog::Release()
{
    if (rectVBO)
    {
        glDeleteBuffers(1, &rectVBO);
        rectVBO = 0;
    }
    if (rectVAO)
    {
        glDeleteVertexArrays(1, &rectVAO);
        rectVAO = 0;
    }
    if (framebufferTexture)
    {
        glDeleteTextures(1, &framebufferTexture);
        framebufferTexture = 0;
    }
    if (depthTexture)
    {
        glDeleteTextures(1, &depthTexture);
        depthTexture = 0;
    }
    if (FBO)
    {
        glDeleteFramebuffers(1, &FBO);
        FBO = 0;
    }
}

void VolumetricFog::Init(int width, int height)
{
    // Create Frame Buffer Object
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    // Create depth texture
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  

    // Post process shader
    framebufferShader.LoadVertexShader("shaders/postprocess/framebuffer.vert.glsl");
    framebufferShader.LoadFragmentShader("shaders/postprocess/framebuffer.frag.glsl");
    framebufferShader.Create();
    framebufferProgram = framebufferShader.GetProgram();
    
    // Set screen texture
    glUseProgram(framebufferProgram);
    glUniform1i(glGetUniformLocation(framebufferProgram, "ScreenTexture"), 0);
    glUniform1i(glGetUniformLocation(framebufferProgram, "DepthTexture"), 1);
    glUseProgram(0);

    // Post process VAO
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void VolumetricFog::Render(ShadowMap* shadowMap, Camera* cam, SceneLightInfo* lightInfo)
{
            // Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Draw FBO color with post process shader
        glUseProgram(framebufferProgram);
		glBindVertexArray(rectVAO);
        
        // Bind screen texture
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glUniform1i(glGetUniformLocation(framebufferProgram, "ScreenTexture"), 0);

        // Bind depth texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(glGetUniformLocation(framebufferProgram, "DepthTexture"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, shadowMap->GetDepthMap());
        glUniform1i(glGetUniformLocation(framebufferProgram, "ShadowMap"), 2);

        glUniform1f(glGetUniformLocation(framebufferProgram, "near"), cam->getNear());
        glUniform1f(glGetUniformLocation(framebufferProgram, "far"), cam->getFar());

        Mat4 invProjection = Mat4::inverse(cam->getProjectionMatrix());
        Mat4 invView = Mat4::inverse(cam->getViewMatrix());
        glUniformMatrix4fv(glGetUniformLocation(framebufferProgram, "cameraInvProjection"), 1, GL_FALSE, invProjection.data());
        glUniformMatrix4fv(glGetUniformLocation(framebufferProgram, "cameraInvView"), 1, GL_FALSE, invView.data());
        glUniformMatrix4fv(glGetUniformLocation(framebufferProgram, "lightMatrix"), 1, GL_FALSE, shadowMap->GetLightSpaceMatrix().data());
        glUniform3f(glGetUniformLocation(framebufferProgram, "cameraPos"), cam->transform.getPosition().x, cam->transform.getPosition().y, cam->transform.getPosition().z);
        glUniform3f(glGetUniformLocation(framebufferProgram, "lightColor"), lightInfo->lightColor.x * lightInfo->lightIntensity, lightInfo->lightColor.y * lightInfo->lightIntensity, lightInfo->lightColor.z * lightInfo->lightIntensity);
        glUniform1f(glGetUniformLocation(framebufferProgram, "time"), Time::time());
        glUniform1f(glGetUniformLocation(framebufferProgram, "lightShaftIntensity"), lightInfo->volumetricIntensity);
        glUniform1f(glGetUniformLocation(framebufferProgram, "lightNoise"), 2.5f);
        glUniform1i(glGetUniformLocation(framebufferProgram, "lightSteps"), 20);
        glDisable(GL_DEPTH_TEST);        
		glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
}