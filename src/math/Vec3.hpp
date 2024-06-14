#include <stdexcept>
#ifndef VEC3_HPP
#define VEC3_HPP

struct Vec3
{
    float x, y, z;
    Vec3();
    Vec3(float _x, float _y, float _z);
    
    float length() const;
    static Vec3 normalize(const Vec3 &vec);
    static Vec3 cross(const Vec3 &a, const Vec3 &b);
    static float dot(const Vec3 &a, const Vec3 &b);

    Vec3 operator+(const Vec3 &other) const;
    Vec3 operator-(const Vec3 &other) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;
    Vec3 operator*(const Vec3 &other) const;
    Vec3 operator/(const Vec3 &other) const;
    Vec3& operator+=(const Vec3 &other);
    Vec3& operator-=(const Vec3 &other);
    Vec3& operator*=(float scalar);
    Vec3& operator/=(float scalar);
    Vec3& operator*=(const Vec3 &other);
    Vec3& operator/=(const Vec3 &other);
};

#endif