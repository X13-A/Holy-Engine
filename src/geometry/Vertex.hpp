#ifndef VERTEX_H
#define VERTEX_H

#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"

struct Vertex
{
    Vec3 position;
    Vec3 color;
    Vec2 uv;
    Vertex(Vec3 _position, Vec3 _color, Vec2 _uv) : position(_position), color(_color), uv(_uv) {}
};

#endif