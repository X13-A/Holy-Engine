#include "Camera.hpp"

// Constructor
Camera::Camera(float fov, float ratio, float near, float far, Vec3 position, Vec3 up)
{
    this->fov = fov;
    this->ratio = ratio;
    this->near = near;
    this->far = far;
    this->position = position;
    this->up = up;
    this->forward = Vec3(0.0f, 0.0f, -1.0f);
    computeProjectionMatrix();
    computeViewMatrix();
    computeAxes();
}

// Sets the target at which the camera should look at
void Camera::setTarget(Vec3 target)
{
    setForward(target - position);
}

// Directly sets the forward vector of the camera
void Camera::setForward(Vec3 forward)
{
    this->forward = Vec3::normalize(forward);
}

// Get forward vector
const Vec3& Camera::getForward() const
{
    return forward;
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
}

// Set aspect ratio
void Camera::setRatio(float ratio)
{
    this->ratio = ratio;
}

// Computes the right and up axes depending on the forward axis
void Camera::computeAxes()
{
    right = Vec3::normalize(Vec3::cross(forward, Vec3(0.0f, 1.0f, 0.0f)));
    up = Vec3::cross(right, forward);
}

// Computes the projection matrix
void Camera::computeProjectionMatrix()
{
    projectionMatrix = Mat4::perspective(fov, ratio, near, far);
}

// Computes the view matrix depending on the camera direction
void Camera::computeViewMatrix()
{
    viewMatrix = Mat4::lookAt(position, position + forward, up);
}

// Get camera position
const Vec3& Camera::getPosition() const
{
    return this->position;
}

// Updates the position of the camera
void Camera::setPosition(Vec3 position)
{
    this->position = position;
}

// Translate the camera in world space
void Camera::translateGlobal(Vec3 offset)
{
    position += offset;
}

// Translates the camera in local space
void Camera::translateLocal(Vec3 offset)
{
    position += right * offset.x;
    position += up * offset.y;
    position += forward * offset.z;
}

// Get camera yaw
float Camera::getYaw() const
{
    float yaw = atan2(forward.z, forward.x);
    return yaw;
}

// Get camera pitch
float Camera::getPitch() const
{
    float pitch = asin(forward.y);
    return pitch;
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
    std::cout << "Pos: " << getPosition().x << ", " << getPosition().y << ", " << getPosition().z << std::endl;
    std::cout << "Rot: " << getForward().x << ", " << getForward().y << ", " << getForward().z << std::endl;
}
