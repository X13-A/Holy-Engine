#include "Vertex.hpp"
#include "../math/Vec2.hpp"
#include "../math/Vec3.hpp"

Vertex::Vertex() : position(Vec3()), normal(Vec3()), texcoord(Vec2()) {}
Vertex::Vertex(Vec3 _position, Vec3 _normal, Vec2 _texcoord) : position(_position), normal(_normal), texcoord(_texcoord) {}