#define TINYOBJLOADER_IMPLEMENTATION
#include "mesh_loader.hpp"
#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"
#include "Vertex.hpp"
#include <unordered_map>

std::vector<Vec2> components_to_vec2s(const std::vector<float> &components)
{
    std::vector<Vec2> res;
    for (size_t vec_start = 0; vec_start < components.size(); vec_start += 2)
    {
        res.push_back(Vec2(components[vec_start], components[vec_start+1]));
    }
    return res;
}

std::vector<Vec3> components_to_vec3s(const std::vector<float>& components) 
{
    std::vector<Vec3> res;
    for(size_t vec_start = 0; vec_start < components.size(); vec_start+=3) 
    {
        res.push_back(Vec3(components[vec_start], components[vec_start+1], components[vec_start+2]));
    }
    return res;
}

bool MeshLoader::load(const std::string& inputfile,
                      std::vector<Vertex>& vertices,
                      std::vector<unsigned int>& indices)
{
    // load the OBJ file
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> objmaterials;
    std::string err;
    std::string warn;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &objmaterials, &warn, &err,
                                    inputfile.c_str(), "./", true);

    std::cout << "Loading model: " << inputfile << std::endl;

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }
    if (!success)
    {
        return false;
    }

    std::unordered_map<std::string, unsigned int> uniqueVertices = {};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {

            // Ensure vertex is within bounds
            if (static_cast<size_t>(index.vertex_index * 3 + 2) >= attrib.vertices.size()) 
            {
                std::cerr << "Vertex index out of bounds: " << index.vertex_index << std::endl;
                continue;
            }

            // Parse vertex
            Vertex vertex;
            vertex.position = Vec3(attrib.vertices[3 * index.vertex_index + 0],
                                   attrib.vertices[3 * index.vertex_index + 1],
                                   attrib.vertices[3 * index.vertex_index + 2]);

            // Parse normal
            if (static_cast<size_t>(index.normal_index * 3 + 2) < attrib.normals.size()) 
            {
                vertex.normal = Vec3(attrib.normals[3 * index.normal_index + 0],
                                     attrib.normals[3 * index.normal_index + 1],
                                     attrib.normals[3 * index.normal_index + 2]);
            }
            else
            {
                vertex.normal = Vec3(0.0f, 0.0f, 0.0f); // Default normal if not present
            }

            // Parse texcoord
            if (static_cast<size_t>(index.texcoord_index * 2 + 1) < attrib.texcoords.size())
            {
                vertex.texcoord = Vec2(attrib.texcoords[2 * index.texcoord_index + 0],
                                       attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else
            {
                vertex.texcoord = Vec2(0.0f, 0.0f); // Default texcoord if not present
            }

            // Create unique key
            std::ostringstream key_ss;
            key_ss << index.vertex_index << "/" << index.normal_index << "/" << index.texcoord_index;
            std::string key = key_ss.str();

            // Add vertice if new
            if (uniqueVertices.count(key) == 0)
            {
                uniqueVertices[key] = static_cast<unsigned int>(vertices.size());
                vertices.push_back(vertex);
            }

            // Add indice
            indices.push_back(uniqueVertices[key]);
        }
    }

    std::cout << "Loaded model: " << inputfile << "." << std::endl;
    return true;
}