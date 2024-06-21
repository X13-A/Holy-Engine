#ifndef MAT4_HPP
#define MAT4_HPP

#include <cmath>
#include <array>
#include "Vec3.hpp"

class Mat4
{
private:
    std::array<float, 16> array;

public:
    // Initialize the matrix as an identity matrix
    Mat4();

    // Access operator
    float& operator()(int row, int col);

    // Access operator (read only)
    const float& operator()(int row, int col) const;

    // Matrix multiplication
    Mat4 operator*(const Mat4& other) const;
    Vec3 operator*(const Vec3& other) const;

    // Get the raw data
    const float* data() const;

    #pragma region Mat4 builders
    static Mat4 rotation_x(float angle);
    static Mat4 rotation_y(float angle);
    static Mat4 rotation_z(float angle);
    static Mat4 scaling(float sx, float sy, float sz);
    static Mat4 translation(float tx, float ty, float tz);
    static Mat4 orthographic(float left, float right, float top, float bottom, float near, float far);
    static Mat4 perspective(float angle, float ratio, float near, float far);
    static Mat4 inverse(const Mat4& mat);
    static Mat4 transpose(const Mat4 &mat);
    static Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

    void scale(float sx, float sy, float sz);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
    void rotate(float x, float y, float z, const Vec3& point = Vec3(0, 0, 0));
    void translate(float tx, float ty, float tz);

    void scale(const Vec3& scale);
    void rotate(const Vec3& rot, const Vec3& point = Vec3(0, 0, 0));
    void translate(const Vec3& offset);

#pragma endregion
};

#endif
