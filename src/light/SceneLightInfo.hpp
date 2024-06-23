#ifndef SCENE_LIGHT_INFO_HPP
#define SCENE_LIGHT_INFO_HPP

#include "../math/Vec3.hpp"

struct SceneLightInfo
{
    Vec3 lightPos;
    Vec3 lightColor;
    Vec3 ambientLight;
    float lightIntensity;
    float volumetricIntensity;
};

#endif