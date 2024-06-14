#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "Vertex.hpp"
#include "../common/GLShader.h"
#include "Transform.hpp"
#include "../camera/Camera.hpp"

class Mesh
{
public:
    GLuint texID;
    GLShader *shader;
    GLuint VBO, VAO, EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void Load(const std::string& filename);
    void Init();
    void Destroy();
    void Draw(Transform& transform, Camera& camera);
    void SetTexture(const std::string& filename);
};

#endif