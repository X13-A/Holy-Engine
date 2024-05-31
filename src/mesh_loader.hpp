#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include "../common/tiny_obj_loader.h"
#include "math/Vec2.hpp"
#include "math/Vec3.hpp"

#include <iostream>
#include <vector>
#include <string>

class MeshLoader
{
public:
    tinyobj::ObjReaderConfig reader_config;
    bool load(const std::string &inputfile,
                     std::vector<Vec3> &vertices,
                     std::vector<Vec3> &normals,
                     std::vector<Vec2> &texcoords);
};

#endif // MESH_LOADER_HPP
