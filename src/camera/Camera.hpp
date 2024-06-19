#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include "../math/Mat4.hpp"
#include "../math/Vec3.hpp"
#include "../geometry/Transform.hpp"

class Camera
{
private:
    float ratio;
    float near;
    float far;
    float fov;
    Mat4 projectionMatrix;

public:

    Transform transform;

    Camera(float fov = 45.0f, float ratio = 8.0f/6.0f, float near = 0.1f, float far = 100.0f, Vec3 position = Vec3(0.0f, 0.0f, 0.0f));

    void setTarget(Vec3 target);
    const Vec3& getForward() const;
    const float& getFov() const;
    void setFov(float fov);
    void setRatio(float ratio);
    void computeProjectionMatrix();
    const Mat4& getProjectionMatrix() const;
    Mat4 getViewMatrix() const;
    const float& getNear() const;
    const float& getFar() const;
    void printValues();
};

#endif // CAMERA_HPP
