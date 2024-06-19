#include "Mat4.hpp"
#include "../math/Math.hpp"
#include <iostream>

Mat4::Mat4()
{
    array.fill(0.0f);
    // Initialize the matrix as an identity matrix
    array[0] = array[5] = array[10] = array[15] = 1.0f;
}

float& Mat4::operator()(int row, int col)
{
    return array[col * 4 + row];
}

const float& Mat4::operator()(int row, int col) const
{
    return array[col * 4 + row];
}

Mat4 Mat4::operator*(const Mat4& other) const 
{
    Mat4 result;
    for (int row = 0; row < 4; ++row) 
    {
        for (int col = 0; col < 4; ++col) 
        {
            result(row, col) = 0.0f;
            for (int k = 0; k < 4; ++k) 
            {
                result(row, col) += (*this)(row, k) * other(k, col);
            }
        }
    }
    return result;
}

Vec3 Mat4::operator*(const Vec3& vec) const 
{
    Vec3 result;
    result.x = (*this)(0, 0) * vec.x + (*this)(0, 1) * vec.y + (*this)(0, 2) * vec.z + (*this)(0, 3);
    result.y = (*this)(1, 0) * vec.x + (*this)(1, 1) * vec.y + (*this)(1, 2) * vec.z + (*this)(1, 3);
    result.z = (*this)(2, 0) * vec.x + (*this)(2, 1) * vec.y + (*this)(2, 2) * vec.z + (*this)(2, 3);
    return result;
}


const float* Mat4::data() const
{
    return array.data();
}

#pragma region Mat4 builders
Mat4 Mat4::rotation_x(float angle)
{
    Mat4 res;
    float cosAngle = cos(radians(angle));
    float sinAngle = sin(radians(angle));

    res(1, 1) = cosAngle;
    res(2, 1) = sinAngle;
    res(1, 2) = -sinAngle;
    res(2, 2) = cosAngle;

    return res;
}

Mat4 Mat4::rotation_y(float angle)
{
    Mat4 res;
    float cosAngle = cos(angle * M_PI / 180.0f);
    float sinAngle = sin(angle * M_PI / 180.0f);

    res(0, 0) = cosAngle;
    res(2, 0) = -sinAngle;
    res(0, 2) = sinAngle;
    res(2, 2) = cosAngle;

    return res;
}

Mat4 Mat4::rotation_z(float angle)
{
    Mat4 res;
    float cosAngle = cos(angle * M_PI / 180.0f);
    float sinAngle = sin(angle * M_PI / 180.0f);

    res(0, 0) = cosAngle;
    res(1, 0) = sinAngle;
    res(0, 1) = -sinAngle;
    res(1, 1) = cosAngle;

    return res;
}

Mat4 Mat4::scaling(float sx, float sy, float sz)
{
    Mat4 res;
    res(0, 0) = sx;
    res(1, 1) = sy;
    res(2, 2) = sz;

    return res;
}

Mat4 Mat4::translation(float tx, float ty, float tz)
{
    Mat4 res;
    res(0, 3) = tx;
    res(1, 3) = ty;
    res(2, 3) = tz;

    return res;
}

Mat4 Mat4::orthographic(float left, float right, float top, float bottom, float near, float far)
{
    Mat4 res;
    res(0, 0) = 2.0 / (right - left);
    res(1, 1) = 2.0 / (top - bottom);
    res(2, 2) = -2.0 / (far - near);

    res(0, 3) = -((right + left) / (right - left));
    res(1, 3) = -((top + bottom) / (top - bottom));
    res(2, 3) = -((far + near) / (far - near));
    res(3, 3) = 1;

    return res;
}

Mat4 Mat4::perspective(float fovy, float ratio, float near, float far)
{
    Mat4 res;
    float tanHalfFovy = tan(radians(fovy) / 2);

    float top = near * tanHalfFovy;
    float bottom = -top;
    float right = top * ratio;
    float left = -right;

    res(0, 0) = (2 * near) / (right - left);
    res(1, 0) = 0;
    res(2, 0) = 0;
    res(3, 0) = 0;

    res(0, 1) = 0;
    res(1, 1) = (2 * near) / (top - bottom);
    res(2, 1) = 0;
    res(3, 1) = 0;

    res(0, 2) = (right + left) / (right - left);
    res(1, 2) = (top + bottom) / (top - bottom);
    res(2, 2) = -(far + near) / (far - near);
    res(3, 2) = -1;

    res(0, 3) = 0;
    res(1, 3) = 0;
    res(2, 3) = -(2 * far * near) / (far - near);
    res(3, 3) = 0;

    return res;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
    // TODO
    std::cerr << "lookAt not implemented" << std::endl;
    exit(1);
    return Mat4();
}

Mat4 Mat4::inverse(const Mat4& m)
{
    Mat4 inv;
    float det;
    int i;

    inv(0, 0) = m(1, 1) * m(2, 2) * m(3, 3) - 
                m(1, 1) * m(2, 3) * m(3, 2) - 
                m(2, 1) * m(1, 2) * m(3, 3) + 
                m(2, 1) * m(1, 3) * m(3, 2) +
                m(3, 1) * m(1, 2) * m(2, 3) - 
                m(3, 1) * m(1, 3) * m(2, 2);

    inv(1, 0) = -m(1, 0) * m(2, 2) * m(3, 3) + 
                 m(1, 0) * m(2, 3) * m(3, 2) + 
                 m(2, 0) * m(1, 2) * m(3, 3) - 
                 m(2, 0) * m(1, 3) * m(3, 2) - 
                 m(3, 0) * m(1, 2) * m(2, 3) + 
                 m(3, 0) * m(1, 3) * m(2, 2);

    inv(2, 0) = m(1, 0) * m(2, 1) * m(3, 3) - 
                m(1, 0) * m(2, 3) * m(3, 1) - 
                m(2, 0) * m(1, 1) * m(3, 3) + 
                m(2, 0) * m(1, 3) * m(3, 1) + 
                m(3, 0) * m(1, 1) * m(2, 3) - 
                m(3, 0) * m(1, 3) * m(2, 1);

    inv(3, 0) = -m(1, 0) * m(2, 1) * m(3, 2) + 
                 m(1, 0) * m(2, 2) * m(3, 1) + 
                 m(2, 0) * m(1, 1) * m(3, 2) - 
                 m(2, 0) * m(1, 2) * m(3, 1) - 
                 m(3, 0) * m(1, 1) * m(2, 2) + 
                 m(3, 0) * m(1, 2) * m(2, 1);

    inv(0, 1) = -m(0, 1) * m(2, 2) * m(3, 3) + 
                 m(0, 1) * m(2, 3) * m(3, 2) + 
                 m(2, 1) * m(0, 2) * m(3, 3) - 
                 m(2, 1) * m(0, 3) * m(3, 2) - 
                 m(3, 1) * m(0, 2) * m(2, 3) + 
                 m(3, 1) * m(0, 3) * m(2, 2);

    inv(1, 1) = m(0, 0) * m(2, 2) * m(3, 3) - 
                m(0, 0) * m(2, 3) * m(3, 2) - 
                m(2, 0) * m(0, 2) * m(3, 3) + 
                m(2, 0) * m(0, 3) * m(3, 2) + 
                m(3, 0) * m(0, 2) * m(2, 3) - 
                m(3, 0) * m(0, 3) * m(2, 2);

    inv(2, 1) = -m(0, 0) * m(2, 1) * m(3, 3) + 
                 m(0, 0) * m(2, 3) * m(3, 1) + 
                 m(2, 0) * m(0, 1) * m(3, 3) - 
                 m(2, 0) * m(0, 3) * m(3, 1) - 
                 m(3, 0) * m(0, 1) * m(2, 3) + 
                 m(3, 0) * m(0, 3) * m(2, 1);

    inv(3, 1) = m(0, 0) * m(2, 1) * m(3, 2) - 
                m(0, 0) * m(2, 2) * m(3, 1) - 
                m(2, 0) * m(0, 1) * m(3, 2) + 
                m(2, 0) * m(0, 2) * m(3, 1) + 
                m(3, 0) * m(0, 1) * m(2, 2) - 
                m(3, 0) * m(0, 2) * m(2, 1);

    inv(0, 2) = m(0, 1) * m(1, 2) * m(3, 3) - 
                m(0, 1) * m(1, 3) * m(3, 2) - 
                m(1, 1) * m(0, 2) * m(3, 3) + 
                m(1, 1) * m(0, 3) * m(3, 2) + 
                m(3, 1) * m(0, 2) * m(1, 3) - 
                m(3, 1) * m(0, 3) * m(1, 2);

    inv(1, 2) = -m(0, 0) * m(1, 2) * m(3, 3) + 
                 m(0, 0) * m(1, 3) * m(3, 2) + 
                 m(1, 0) * m(0, 2) * m(3, 3) - 
                 m(1, 0) * m(0, 3) * m(3, 2) - 
                 m(3, 0) * m(0, 2) * m(1, 3) + 
                 m(3, 0) * m(0, 3) * m(1, 2);

    inv(2, 2) = m(0, 0) * m(1, 1) * m(3, 3) - 
                m(0, 0) * m(1, 3) * m(3, 1) - 
                m(1, 0) * m(0, 1) * m(3, 3) + 
                m(1, 0) * m(0, 3) * m(3, 1) + 
                m(3, 0) * m(0, 1) * m(1, 3) - 
                m(3, 0) * m(0, 3) * m(1, 1);

    inv(3, 2) = -m(0, 0) * m(1, 1) * m(3, 2) + 
                 m(0, 0) * m(1, 2) * m(3, 1) + 
                 m(1, 0) * m(0, 1) * m(3, 2) - 
                 m(1, 0) * m(0, 2) * m(3, 1) - 
                 m(3, 0) * m(0, 1) * m(1, 2) + 
                 m(3, 0) * m(0, 2) * m(1, 1);

    inv(0, 3) = -m(0, 1) * m(1, 2) * m(2, 3) + 
                 m(0, 1) * m(1, 3) * m(2, 2) + 
                 m(1, 1) * m(0, 2) * m(2, 3) - 
                 m(1, 1) * m(0, 3) * m(2, 2) - 
                 m(2, 1) * m(0, 2) * m(1, 3) + 
                 m(2, 1) * m(0, 3) * m(1, 2);

    inv(1, 3) = m(0, 0) * m(1, 2) * m(2, 3) - 
                m(0, 0) * m(1, 3) * m(2, 2) - 
                m(1, 0) * m(0, 2) * m(2, 3) + 
                m(1, 0) * m(0, 3) * m(2, 2) + 
                m(2, 0) * m(0, 2) * m(1, 3) - 
                m(2, 0) * m(0, 3) * m(1, 2);

    inv(2, 3) = -m(0, 0) * m(1, 1) * m(2, 3) + 
                 m(0, 0) * m(1, 3) * m(2, 1) + 
                 m(1, 0) * m(0, 1) * m(2, 3) - 
                 m(1, 0) * m(0, 3) * m(2, 1) - 
                 m(2, 0) * m(0, 1) * m(1, 3) + 
                 m(2, 0) * m(0, 3) * m(1, 1);

    inv(3, 3) = m(0, 0) * m(1, 1) * m(2, 2) - 
                m(0, 0) * m(1, 2) * m(2, 1) - 
                m(1, 0) * m(0, 1) * m(2, 2) + 
                m(1, 0) * m(0, 2) * m(2, 1) + 
                m(2, 0) * m(0, 1) * m(1, 2) - 
                m(2, 0) * m(0, 2) * m(1, 1);

    det = m(0, 0) * inv(0, 0) + m(0, 1) * inv(1, 0) + m(0, 2) * inv(2, 0) + m(0, 3) * inv(3, 0);

    if (det == 0)
        throw std::runtime_error("Matrix is not invertible");

    det = 1.0 / det;

    for (i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            inv(i, j) = inv(i, j) * det;
        }
    }

    return inv;
}

void Mat4::scale(float sx, float sy, float sz)
{
    Mat4 scalingMatrix = Mat4::scaling(sx, sy, sz);
    *this = *this * scalingMatrix;
}

void Mat4::rotate_x(float angle)
{
    Mat4 rotationMatrix = Mat4::rotation_x(angle);
    *this = *this * rotationMatrix;
}

void Mat4::rotate_y(float angle)
{
    Mat4 rotationMatrix = Mat4::rotation_y(angle);
    *this = *this * rotationMatrix;
}

void Mat4::rotate_z(float angle)
{
    Mat4 rotationMatrix = Mat4::rotation_z(angle);
    *this = *this * rotationMatrix;
}

void Mat4::translate(float tx, float ty, float tz)
{
    Mat4 translationMatrix = Mat4::translation(tx, ty, tz);
    *this = *this * translationMatrix;
}

void Mat4::rotate(float x, float y, float z, const Vec3& point)
{
    // Translate to the point
    Mat4 translationToPoint = Mat4::translation(-point.x, -point.y, -point.z);
    *this = *this * translationToPoint;

    // Apply the rotations (Tait-Bryan angles)
    Mat4 rotationMatrix_x = Mat4::rotation_x(x);
    Mat4 rotationMatrix_y = Mat4::rotation_y(y);
    Mat4 rotationMatrix_z = Mat4::rotation_z(z);

    *this = *this * rotationMatrix_z;
    *this = *this * rotationMatrix_y;
    *this = *this * rotationMatrix_x;

    // Translate back from the point
    Mat4 translationBack = Mat4::translation(point.x, point.y, point.z);
    *this = *this * translationBack;
}

void Mat4::scale(const Vec3& scale)
{
    this->scale(scale.x, scale.y, scale.z);
}
void Mat4::rotate(const Vec3& rot, const Vec3& point)
{
    this->rotate(rot.x, rot.y, rot.z, point);
}
void Mat4::translate(const Vec3& offset)
{
    this->translate(offset.x, offset.y, offset.z);
}

#pragma endregion
