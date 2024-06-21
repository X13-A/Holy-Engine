#include "Model.hpp"
#include "../materials/Material.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Transform.hpp"
#include "../geometry/Vertex.hpp"
#include "../math/Mat4.hpp";
#include "../geometry/ModelLoader.hpp"

void Model::Init()
{
    for (Shape* shape : shapes)
    {
        shape->Init();
    }
}

void Model::Draw(Camera* camera)
{
    for (Shape* shape : shapes)
    {
        shape->Draw(camera, transform);
    }
}