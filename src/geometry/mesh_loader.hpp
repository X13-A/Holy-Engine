#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include "../common/tiny_obj_loader.h"
#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"
#include "Vertex.hpp"

#include <iostream>
#include <vector>
#include <string>

class MeshLoader
{
public:
    tinyobj::ObjReaderConfig reader_config;
    bool load(const std::string& inputfile,
                      std::vector<Vertex>& vertices,
                      std::vector<unsigned int>& indices);
};

#endif // MESH_LOADER_HPP
