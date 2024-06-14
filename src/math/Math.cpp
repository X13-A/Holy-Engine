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
        radians(degrees.y),
        radians(degrees.z)
    );
}

float degrees(float radians)
{
    return radians * (180.0f / M_PI);
}

Vec3 degrees(const Vec3& radians)
{
    return Vec3(
        degrees(radians.x),
        degrees(radians.y),
        degrees(radians.z)
    );
}