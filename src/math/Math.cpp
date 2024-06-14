#include "Math.hpp"
#include "Vec3.hpp"
#include <cmath>

float radians(float degrees)
{
    return degrees * (M_PI / 180.0f);
}

Vec3 radians(const Vec3& degrees)
{
    return Vec3(
        radians(degrees.x),
        radians(degrees.x),
        radians(degrees.z)
    );
}