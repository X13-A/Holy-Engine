#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "Vertex.hpp"
#include "../common/GLShader.h"

class Mesh
{
public:
    GLuint texID1, texID2;
    GLShader *shader;
    GLuint VBO, VAO, EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void Load(const std::string& filename);
    void Init();
    void Destroy();
    void Draw();
    void SetTexture(const std::string& filename, int slot = 0);
};

#endif