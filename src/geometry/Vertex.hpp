#ifndef VERTEX_H
#define VERTEX_H

#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texcoord;
    Vertex();
    Vertex(Vec3 _position, Vec3 _normal, Vec2 _texcoord);
};

#endif