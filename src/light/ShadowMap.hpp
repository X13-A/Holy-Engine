#ifndef SHADOW_MAP_HPP
#define SHADOW_MAP_HPP
#include "SceneLightInfo.hpp"
#include "../common/GLShader.h"
#include <vector>
#include "../model/Model.hpp"
#include "../math/Mat4.hpp"

class ShadowMap
{
private:
    SceneLightInfo* lightInfo;
    GLShader* shader;
    unsigned int width, height;
    GLuint FBO;
    GLuint depthMap;
    Mat4 projectionMatrix;
    Mat4 viewMatrix;
    Mat4 lightSpaceMatrix;
    Vec3 target;
    void CreateShader();

public:
    GLuint GetDepthMap() const;
    const Mat4& GetLightSpaceMatrix() const;
    void Create();
    void Compute(std::vector<Model*>& models);
    void Attach(SceneLightInfo *lightInfo);
};

#endif