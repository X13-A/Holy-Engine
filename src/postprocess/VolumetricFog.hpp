#ifndef VOLUMETRIC_FOG_HPP
#define VOLUMETRIC_FOG_HPP
#include <GL/glew.h>
#include "../common/GLShader.h"
#include "../camera/Camera.hpp"
#include "../light/ShadowMap.hpp"
#include "../light/SceneLightInfo.hpp"

const float screenVertices[6 * 4] =
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

class VolumetricFog
{
public:
    GLuint FBO;
    GLuint framebufferTexture;
    GLuint depthTexture;
    GLShader framebufferShader;
    GLuint framebufferProgram;
    GLuint rectVAO, rectVBO;

    VolumetricFog();
    ~VolumetricFog();
    void Release();

    void Init(int width, int height);
    void Render(ShadowMap *shadowMap, Camera *cam, SceneLightInfo *lightInfo);
};
#endif