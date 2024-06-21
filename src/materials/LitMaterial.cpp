#include "LitMaterial.hpp"
#include "../geometry/Transform.hpp"
#include "../camera/Camera.hpp"
#include "../light/SceneLightInfo.hpp"

void LitMaterial::SetUniforms()
{
    Material::SetUniforms();

    // Lighting 
    auto loc_lightPos = glGetUniformLocation(shader->GetProgram(), "lightPos");
    glUniform3f(loc_lightPos, lightInfo->lightPos.x, lightInfo->lightPos.y, lightInfo->lightPos.z);

    auto loc_lightColor = glGetUniformLocation(shader->GetProgram(), "lightColor");
    glUniform3f(loc_lightColor, lightInfo->lightColor.x, lightInfo->lightColor.y, lightInfo->lightColor.z);

    auto loc_ambientLight = glGetUniformLocation(shader->GetProgram(), "ambientLight");
    glUniform3f(loc_ambientLight, lightInfo->ambientLight.x, lightInfo->ambientLight.y, lightInfo->ambientLight.z);

    // Camera
    Vec3 camPos = camera->transform.getPosition();
    auto loc_camPos = glGetUniformLocation(shader->GetProgram(), "camPos");
    glUniform3f(loc_camPos, camPos.x, camPos.y, camPos.z);

    // Material specific uniforms
    auto loc_metallic = glGetUniformLocation(shader->GetProgram(), "metallic");
    glUniform1f(loc_metallic, metallic);

    auto loc_smoothness = glGetUniformLocation(shader->GetProgram(), "smoothness");
    glUniform1f(loc_smoothness, smoothness);
}

void LitMaterial::Attach(Camera *camera, SceneLightInfo* lightInfo)
{
    this->camera = camera;
    this->lightInfo = lightInfo;
}