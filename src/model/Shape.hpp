#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "../geometry/Mesh.hpp"
#include "../materials/Material.hpp"

class Shape
{
public:
    Mesh* mesh;
    Material* material;
    void Init();
    void Destroy();
    void Draw(Camera *camera, Transform *transform);
};

#endif