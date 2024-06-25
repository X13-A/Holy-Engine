#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "../geometry/Mesh.hpp"
#include "../materials/Material.hpp"

class Shape
{
public:
    Mesh* mesh;
    Material* material;
    
    Shape();
    ~Shape();
    
    void Init();
    void Destroy();
    void Draw(Camera *camera, Transform *transform);
    void Release();
};

#endif