#ifndef LIT_MATERIAL_HPP
#define LIT_MATERIAL_HPP
#include "Material.hpp"
#include "../geometry/Transform.hpp"
#include "../camera/Camera.hpp"
#include "../light/SceneLightInfo.hpp"

class LitMaterial : public Material
{
public:
    float roughness;
    float metallic;

    // External parameters
    Camera* camera;
    SceneLightInfo *lightInfo;

    void Attach(Camera *camera, SceneLightInfo* lightInfo);
    void SetUniforms() override;
};

#endif
