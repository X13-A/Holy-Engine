#define TINYOBJLOADER_IMPLEMENTATION
#include "mesh_loader.hpp"
#include "math/Vec2.hpp"
#include "math/Vec3.hpp"

bool MeshLoader::load(const std::string& inputfile,
                    std::vector<Vec3>& vertices,
                    std::vector<Vec3>& normals,
                    std::vector<Vec2>& texcoords)
{
    reader_config.mtl_search_path = "./";
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) 
    {
        if (!reader.Error().empty()) 
        {
            std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
        }
        return false;
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning() << std::endl;
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    for (const auto& shape : shapes) 
    {
        size_t index_offset = 0;
        for (const auto num_face_vertices : shape.mesh.num_face_vertices) 
        {
            size_t fv = static_cast<size_t>(num_face_vertices);

            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                
                vertices.push_back({
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                });

                if (idx.normal_index >= 0)
                {
                    normals.push_back({
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    });
                }

                if (idx.texcoord_index >= 0)
                {
                    texcoords.push_back({
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        attrib.texcoords[2 * idx.texcoord_index + 1]
                    });
                }
            }
            index_offset += fv;
        }
    }

    return true;
}