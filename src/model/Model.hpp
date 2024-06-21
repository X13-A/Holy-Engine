#ifndef MODEL_HPP
#define MODEL_HPP

#include "../materials/Material.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Transform.hpp"
#include "Shape.hpp"
#include <vector>

class Model
{
public:
    Transform* transform;
    std::vector<Shape*> shapes;

    void Init();
    void Draw(Camera *camera);
};

#endif