#include "ShadowMap.hpp"
#include "SceneLightInfo.hpp"
#include "GL/glew.h"

void ShadowMap::Attach(SceneLightInfo* lightInfo)
{
    this->lightInfo = lightInfo;
}

void ShadowMap::CreateShader()
{
    const char *vert_path = "shaders/shadowMap/vert.glsl";
    const char *frag_path = "shaders/shadowMap/frag.glsl";
    shader = new GLShader();
    if (!shader->LoadVertexShader(vert_path))
    {
        std::cerr << "Failed to load vertex shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->LoadFragmentShader(frag_path))
    {
        std::cerr << "Failed to load fragment shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->Create())
    {
        std::cerr << "Failed to create shader program" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ShadowMap::Create()
{
    CreateShader();

    // Configure bounds
    // TODO: finetune projection
    float size = 30;
    float left = -size, right = size;
    float top = size, bottom = -size;
    float near = -0.1f, far = 50.0f;
    projectionMatrix = Mat4::orthographic(left, right, top, bottom, near, far);

    width = 4096, height = 4096; // We are rich
    glGenFramebuffers(1, &FBO); 
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void ShadowMap::Compute(std::vector<Model*>& models)
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    uint32_t shaderProgram = shader->GetProgram();
    glUseProgram(shaderProgram);

    // TODO: Only compute when light pos changes
    viewMatrix = Mat4::lookAt(lightInfo->lightPos, target, Vec3(0, 1, 0));
    lightSpaceMatrix = projectionMatrix * viewMatrix; 

    auto loc_viewMatrix = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, viewMatrix.data());

    auto loc_projectionMatrix = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(loc_projectionMatrix, 1, GL_FALSE, projectionMatrix.data());

    // Draw all the meshes
    for (Model* model : models)
    {
        for (Shape* shape : model->shapes)
        {
            auto loc_modelMatrix = glGetUniformLocation(shaderProgram, "modelMatrix");
            glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, model->transform->getTransformMatrix().data());

            // Hacky because these VAO's use normals & uvs which are not used here, so they might get optimized out.
            // Shader uses them to prevent their removal.
            glBindVertexArray(shape->mesh->VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(shape->mesh->indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
    // Shadowmap is drawn, just use depth map.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
}

const Mat4& ShadowMap::GetLightSpaceMatrix() const
{
    return lightSpaceMatrix;
}

GLuint ShadowMap::GetDepthMap() const
{
    return depthMap;
}
