#include "Camera.hpp"
#include <iostream>

// Constructor
Camera::Camera(float fov, float ratio, float near, float far, Vec3 position) : fov(fov), ratio(ratio), near(near), far(far), transform(position, Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f))
{
    computeProjectionMatrix();
    computeViewMatrix();
}

// Sets the target at which the camera should look at
void Camera::setTarget(Vec3 target)
{
    Vec3 forward = Vec3::normalize(target - transform.getPosition());
    transform.setRotation(forward);
}

// Get forward vector
const Vec3& Camera::getForward() const
{
    return transform.getRotation();
}

// Get FOV
const float& Camera::getFov() const
{
    return fov;
}

// Set FOV
void Camera::setFov(float fov)
{
    this->fov = fov;
    computeProjectionMatrix();
}

// Set aspect ratio
void Camera::setRatio(float ratio)
{
    this->ratio = ratio;
    computeProjectionMatrix();
}

// Computes the projection matrix
void Camera::computeProjectionMatrix()
{
    projectionMatrix = Mat4::perspective(fov, ratio, near, far);
}

// Computes the view matrix depending on the camera direction
void Camera::computeViewMatrix()
{
    Vec3 defaultForward(0.0f, 0.0f, -1.0f);

    Vec3 position = transform.getPosition();
    Mat4 R;
    R.rotate(transform.getRotation());
    Vec3 forward = R * defaultForward;
    Vec3 up = Vec3::cross(Vec3::normalize(Vec3::cross(forward, Vec3(0.0f, 1.0f, 0.0f))), forward);
    viewMatrix = Mat4::lookAt(position, position + forward, up);
}

// Get near plane
float Camera::getNear() const
{
    return near;
}

// Get far plane
float Camera::getFar() const
{
    return far;
}

// Print camera values
void Camera::printValues()
{
    std::cout << "Pos: " << transform.getPosition().x << ", " << transform.getPosition().y << ", " << transform.getPosition().z << std::endl;
    std::cout << "Forward: " << transform.getRotation().x << ", " << transform.getRotation().y << ", " << transform.getRotation().z << std::endl;
}
