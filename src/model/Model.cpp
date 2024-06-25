#include "Model.hpp"
#include "../materials/Material.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Transform.hpp"
#include "../geometry/Vertex.hpp"
#include "../math/Mat4.hpp";
#include "../geometry/ModelLoader.hpp"

Model::Model() : transform(nullptr)
{
}

Model::~Model()
{
    Release();
}

void Model::Release()
{
    if (transform)
    {
        delete transform;
        transform = nullptr;
    }

    for (Shape* shape : shapes)
    {
        if (shape)
        {
            shape->Release();
            delete shape;
        }
    }
    shapes.clear();
}


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