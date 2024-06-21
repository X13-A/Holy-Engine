#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include "../common/tiny_obj_loader.h"
#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"
#include "../model/Shape.hpp"
#include "../light/SceneLightInfo.hpp"
#include "Vertex.hpp"

#include <iostream>
#include <vector>
#include <string>

class ModelLoader
{
public:
    tinyobj::ObjReaderConfig reader_config;
    bool load(const std::string &inputObj, const std::string &inputMtl, std::vector<Shape*>& shapes, Camera* camera, SceneLightInfo* lightInfo);
};

#endif // MESH_LOADER_HPP
