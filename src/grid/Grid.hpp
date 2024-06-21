#ifndef GRID_HPP
#define GRID_HPP

#include <GL/glew.h>
#include <vector>
#include "Vertex.hpp"
#include "../common/GLShader.h"
#include "../camera/Camera.hpp"

class WireFrame
{
public:
    GLShader *shader;
    GLuint VBO, VAO, EBO;

    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;

    void GenerateGrid(int size, float spacing);
    void Init();
    void Destroy();
    void Draw(Camera& camera);
};

#endif