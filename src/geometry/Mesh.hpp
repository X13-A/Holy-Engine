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
    GLuint VBO, VAO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void Init();
    void Destroy();
};

#endif