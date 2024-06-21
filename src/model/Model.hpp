#ifndef MODEL_HPP
#define MODEL_HPP

#include "../materials/Material.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Transform.hpp"

class Model
{
public:
    Transform* transform;
    Mesh* mesh;
    Material* material;

    void Init();
    void Draw(Camera *camera);
};

#endif