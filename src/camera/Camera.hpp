#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include "../math/Mat4.hpp"
#include "../math/Vec3.hpp"

class Camera
{
private:
    float ratio;
    float near;
    float far;
    Vec3 position;
    
    Vec3 forward;
    Vec3 up;
    Vec3 right;
    float fov;

public:
    Mat4 projectionMatrix;
    Mat4 viewMatrix;

    Camera(float fov = 45, float ratio = 8/6, float near = 0.1f, float far = 100.0f, Vec3 position = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f));

    void setTarget(Vec3 target);
    void setForward(Vec3 forward);
    const Vec3& getForward() const;
    const float& getFov() const;
    void setFov(float fov);
    void setRatio(float ratio);
    void computeAxes();
    void computeProjectionMatrix();
    void computeViewMatrix();
    const Vec3& getPosition() const;
    void setPosition(Vec3 position);
    void translateGlobal(Vec3 offset);
    void translateLocal(Vec3 offset);
    float getYaw() const;
    float getPitch() const;
    float getNear() const;
    float getFar() const;
    void printValues();
};

#endif // CAMERA_HPP
