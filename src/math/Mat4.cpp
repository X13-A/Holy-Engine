#include "Mat4.hpp"

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
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result(i, j) = 0;
            for (int k = 0; k < 4; ++k)
            {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }
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
    float cosAngle = cos(angle * M_PI / 180.0f);
    float sinAngle = sin(angle * M_PI / 180.0f);

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
    float cot = 1/(tan(fovy / 2));

    res(0, 0) = cot / ratio;
    res(1, 1) = cot;
    res(2, 2) = -(far / (far - near));
    res(3, 2) = -1;
    res(2, 3) = -((near * far) / (far - near));

    return res;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
{
    Vec3 f = Vec3::normalize(center - eye);
    Vec3 u = Vec3::normalize(up);
    Vec3 s = Vec3::normalize(Vec3::cross(f, u));
    u = Vec3::cross(s, f);

    Mat4 lookAt;
    lookAt(0, 0) = s.x;
    lookAt(0, 1) = s.y;
    lookAt(0, 2) = s.z;
    lookAt(0, 3) = -Vec3::dot(s, eye);

    lookAt(1, 0) = u.x;
    lookAt(1, 1) = u.y;
    lookAt(1, 2) = u.z;
    lookAt(1, 3) = -Vec3::dot(u, eye);

    lookAt(2, 0) = -f.x;
    lookAt(2, 1) = -f.y;
    lookAt(2, 2) = -f.z;
    lookAt(2, 3) = Vec3::dot(f, eye);

    lookAt(3, 0) = 0.0f;
    lookAt(3, 1) = 0.0f;
    lookAt(3, 2) = 0.0f;
    lookAt(3, 3) = 1.0f;

    return lookAt;
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

void Mat4::rotate(float x, float y, float z)
{
    Mat4 rotationMatrix_x = Mat4::rotation_x(x);
    Mat4 rotationMatrix_y = Mat4::rotation_y(y);
    Mat4 rotationMatrix_z = Mat4::rotation_z(z);

    // Apply the rotations (Tait-Bryan angles)
    *this = *this * rotationMatrix_z;
    *this = *this * rotationMatrix_y;
    *this = *this * rotationMatrix_x;
}


void Mat4::scale(const Vec3& scale)
{
    this->scale(scale.x, scale.y, scale.z);
}
void Mat4::rotate(const Vec3& rot)
{
    this->rotate(rot.x, rot.y, rot.z);
}
void Mat4::translate(const Vec3& offset)
{
    this->translate(offset.x, offset.y, offset.z);
}

#pragma endregion
