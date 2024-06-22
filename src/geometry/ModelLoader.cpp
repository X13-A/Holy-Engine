#define TINYOBJLOADER_IMPLEMENTATION
#include "ModelLoader.hpp"
#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"
#include "Vertex.hpp"
#include <unordered_map>
#include <vector>
#include "../model/Shape.hpp"
#include "../materials/LitMaterial.hpp"
#include "../camera/Camera.hpp"
#include "../light/SceneLightInfo.hpp"
#include "../light/ShadowMap.hpp"

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

// Loads a model made of shapes with LitMaterials
// TODO: Last parameters really should not be there. Must find a better way to attach objects to the material.
bool ModelLoader::load(const std::string &inputObj, const std::string &inputMtl, std::vector<Shape*>& shapes, Camera* camera, SceneLightInfo* lightInfo, ShadowMap* shadowMap)
{
    // load the OBJ file
    tinyobj::attrib_t tiny_obj_attrib;
    std::vector<tinyobj::shape_t> tiny_obj_shapes;
    std::vector<tinyobj::material_t> tiny_obj_materials;
    std::string err;
    std::string warn;

    bool success = tinyobj::LoadObj(&tiny_obj_attrib, &tiny_obj_shapes, &tiny_obj_materials, &warn, &err, inputObj.c_str(), inputMtl.c_str(), true);

    std::cout << "Loading model: " << inputObj << std::endl;

    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }
    if (!success)
    {
        return false;
    }


    for (const auto& shape : tiny_obj_shapes)
    {
        // Extract Mesh
        std::unordered_map<std::string, unsigned int> uniqueVertices = {};
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (const auto& index : shape.mesh.indices)
        {
            // Ensure vertex is within bounds
            if (static_cast<size_t>(index.vertex_index * 3 + 2) >= tiny_obj_attrib.vertices.size()) 
            {
                std::cerr << "Vertex index out of bounds: " << index.vertex_index << std::endl;
                continue;
            }

            // Parse vertex
            Vertex vertex;
            vertex.position = Vec3(tiny_obj_attrib.vertices[3 * index.vertex_index + 0],
                                   tiny_obj_attrib.vertices[3 * index.vertex_index + 1],
                                   tiny_obj_attrib.vertices[3 * index.vertex_index + 2]);

            // Parse normal
            if (static_cast<size_t>(index.normal_index * 3 + 2) < tiny_obj_attrib.normals.size()) 
            {
                vertex.normal = Vec3(tiny_obj_attrib.normals[3 * index.normal_index + 0],
                                     tiny_obj_attrib.normals[3 * index.normal_index + 1],
                                     tiny_obj_attrib.normals[3 * index.normal_index + 2]);
            }
            else
            {
                vertex.normal = Vec3(0.0f, 0.0f, 0.0f); // Default normal if not present
            }

            // Parse texcoord
            if (static_cast<size_t>(index.texcoord_index * 2 + 1) < tiny_obj_attrib.texcoords.size())
            {
                vertex.texcoord = Vec2(tiny_obj_attrib.texcoords[2 * index.texcoord_index + 0],
                                       tiny_obj_attrib.texcoords[2 * index.texcoord_index + 1]);
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
        Mesh *mesh = new Mesh();
        mesh->indices = indices;
        mesh->vertices = vertices;
        
        // Extract Material
        LitMaterial *material = new LitMaterial();
        material->albedoPath = ""; // Default components
        material->roughness = 1;
        material->metallic = 0;

        int material_id = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];
        if (material_id >= 0 && material_id < tiny_obj_materials.size())
        {
            tinyobj::material_t tiny_obj_material = tiny_obj_materials[material_id];
            
            if (tiny_obj_material.diffuse_texname != "")
            {
                material->albedoPath = inputMtl + "/" + tiny_obj_material.diffuse_texname;
            }
            material->roughness = tiny_obj_material.roughness;
            material->metallic = tiny_obj_material.metallic;
        }
        material->Attach(camera, lightInfo, shadowMap); // TODO: Used for PBR rendering. This operation should be moved elsewhere.

        Shape *res_shape = new Shape();
        res_shape->mesh = mesh;
        res_shape->material = material;
        shapes.push_back(res_shape);
    }

    std::cout << "Loaded model: " << inputObj << "." << std::endl;
    return true;
}