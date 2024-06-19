#include "Vec3.hpp"
#include <cmath>
#include <stdio.h>
#include <iostream>

Vec3::Vec3() : Vec3(0, 0, 0) {}
Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

float Vec3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize(const Vec3& vec)
{
    float length = vec.length();
    if (length == 0)
    {
        return vec;
    }
    return vec / length;
}

Vec3 Vec3::cross(const Vec3 &a, const Vec3 &b)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float Vec3::dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


Vec3 Vec3::operator+(const Vec3 &other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(float scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(float scalar) const
{
    if (scalar != 0)
    {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }
    throw std::invalid_argument("Division by zero");
}

Vec3 Vec3::operator*(const Vec3 &other) const
{
    return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 Vec3::operator/(const Vec3 &other) const
{
    if (other.x != 0 && other.y != 0 && other.z != 0)
    {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }
    throw std::invalid_argument("Division by zero");
}

Vec3& Vec3::operator+=(const Vec3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3& Vec3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vec3& Vec3::operator/=(float scalar)
{
    if (scalar != 0)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    throw std::invalid_argument("Division by zero");
}

Vec3& Vec3::operator*=(const Vec3& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

Vec3& Vec3::operator/=(const Vec3& other)
{
    if (other.x != 0 && other.y != 0 && other.z != 0)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    throw std::invalid_argument("Division by zero");
}

float Vec3::distance(const Vec3& a, const Vec3& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

void Vec3::printValues() const
{
    std::cout << "x: " << x << ", y: " << y << ", z: " << z << std::endl;
}