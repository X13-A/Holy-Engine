#include "CreativeControls.hpp"
#include "../input/InputManager.hpp"
#include "../math/Mat4.hpp"

CreativeControls::CreativeControls(Camera* camera, float moveSpeed, float rotateSpeed) : CameraControls(camera), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed)
{
    camera->transform.lookat(camera->transform.getPosition() + Vec3(0, 0, -1));
    // if (camera)
    // {
    //     yaw = camera->transform.getYaw();
    //     pitch = camera->transform.getPitch();
    // }
    // else
    // {
    //     std::cerr << "Camera is null in CreativeControls constructor" << std::endl;
    // }
}

void CreativeControls::handleMouseMove(float xoffset, float yoffset)
{
    yaw += xoffset * rotateSpeed;
    pitch += yoffset * rotateSpeed;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    Vec3 direction;
    direction.x = -cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));

    direction = Vec3::normalize(degrees(direction));
    camera->transform.lookat(camera->transform.getPosition() + direction);
}

void CreativeControls::handleScroll(float xoffset, float yoffset)
{
    float fov = camera->getFov() - yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;
    camera->setFov(fov);
    camera->computeProjectionMatrix();
}

void CreativeControls::update(InputManager* inputManager)
{
    if (inputManager->isKeyPressed(KeyboardKey::Space))
    {
        camera->transform.translate(Vec3(0, moveSpeed * Time::deltaTime(), 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::LSHIFT))
    {
        camera->transform.translate(Vec3(0, -moveSpeed * Time::deltaTime(), 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::A))
    {
        camera->transform.translate(Vec3(-moveSpeed * Time::deltaTime(), 0, 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::D))
    {
        camera->transform.translate(Vec3(moveSpeed * Time::deltaTime(), 0, 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::W))
    {
        camera->transform.translate(Vec3(0, 0, moveSpeed * Time::deltaTime()));
    }
    if (inputManager->isKeyPressed(KeyboardKey::S))
    {
        camera->transform.translate(Vec3(0, 0, -moveSpeed * Time::deltaTime()));
    }
}