#include "LitMaterial.hpp"
#include "../geometry/Transform.hpp"
#include "../camera/Camera.hpp"
#include "../light/SceneLightInfo.hpp"
#include "../light/ShadowMap.hpp"

void LitMaterial::SetUniforms()
{
    Material::SetUniforms();
    uint32_t shaderProgram = shader->GetProgram();
    // Lighting
    auto loc_lightPos = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(loc_lightPos, lightInfo->lightPos.x, lightInfo->lightPos.y, lightInfo->lightPos.z);

    auto loc_lightColor = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(loc_lightColor, lightInfo->lightColor.x, lightInfo->lightColor.y, lightInfo->lightColor.z);

    auto loc_ambientLight = glGetUniformLocation(shaderProgram, "ambientLight");
    glUniform3f(loc_ambientLight, lightInfo->ambientLight.x, lightInfo->ambientLight.y, lightInfo->ambientLight.z);

    // Shadow map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMap->GetDepthMap());
    auto loc_texture = glGetUniformLocation(shaderProgram, "shadowMap");
    glUniform1i(loc_texture, 1);

    auto loc_lightMatrix = glGetUniformLocation(shaderProgram, "lightMatrix");
    glUniformMatrix4fv(loc_lightMatrix, 1, GL_FALSE, shadowMap->GetLightSpaceMatrix().data());

    // Camera
    Vec3 camPos = camera->transform.getPosition();
    auto loc_camPos = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3f(loc_camPos, camPos.x, camPos.y, camPos.z);

    // Material specific uniforms
    auto loc_metallic = glGetUniformLocation(shaderProgram, "metallic");
    glUniform1f(loc_metallic, metallic);

    auto loc_roughness = glGetUniformLocation(shaderProgram, "roughness");
    glUniform1f(loc_roughness, roughness);
}

void LitMaterial::Attach(Camera *camera, SceneLightInfo* lightInfo, ShadowMap* shadowMap)
{
    this->camera = camera;
    this->lightInfo = lightInfo;
    this->shadowMap = shadowMap;
}