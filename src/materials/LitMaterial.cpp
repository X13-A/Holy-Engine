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
    Vec3 lightDir = Vec3::normalize(lightInfo->lightPos * -1);
    auto loc_lightPos = glGetUniformLocation(shaderProgram, "lightDir");
    glUniform3f(loc_lightPos, lightDir.x, lightDir.y, lightDir.z);

    auto loc_lightColor = glGetUniformLocation(shaderProgram, "lightColor");
    glUniform3f(loc_lightColor, lightInfo->lightColor.x * lightInfo->lightIntensity, lightInfo->lightColor.y * lightInfo->lightIntensity, lightInfo->lightColor.z * lightInfo->lightIntensity);

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
    auto loc_metallic = glGetUniformLocation(shaderProgram, "metallicUniform");
    glUniform1f(loc_metallic, metallic);

    auto loc_roughness = glGetUniformLocation(shaderProgram, "roughnessUniform");
    glUniform1f(loc_roughness, roughness);

    // Normal map
    auto loc_hasNormalMap = glGetUniformLocation(shaderProgram, "hasNormalMap");
    glUniform1i(loc_hasNormalMap, hasNormalMap ? 1 : 0);
    if (hasNormalMap)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalTexID);
        auto loc_texture = glGetUniformLocation(shader->GetProgram(), "normalMap");
        glUniform1i(loc_texture, 1);
    }

    // Roughness map
    auto loc_hasRoughnessMap = glGetUniformLocation(shaderProgram, "hasRoughnessMap");
    glUniform1i(loc_hasRoughnessMap, hasRoughnessMap ? 1 : 0);
    if (hasRoughnessMap)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, roughnessTexID);
        auto loc_texture = glGetUniformLocation(shader->GetProgram(), "roughnessMap");
        glUniform1i(loc_texture, 2);
    }

    // Metallic map
    auto loc_hasMetallicMap = glGetUniformLocation(shaderProgram, "hasMetallicMap");
    glUniform1i(loc_hasMetallicMap, hasMetallicMap ? 1 : 0);
    if (hasMetallicMap)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, metallicTexID);
        auto loc_texture = glGetUniformLocation(shader->GetProgram(), "metallicMap");
        glUniform1i(loc_texture, 3);
    }
}

void LitMaterial::Attach(Camera *camera, SceneLightInfo* lightInfo, ShadowMap* shadowMap)
{
    this->camera = camera;
    this->lightInfo = lightInfo;
    this->shadowMap = shadowMap;
}