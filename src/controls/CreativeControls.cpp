#include "CreativeControls.hpp"
#include "../input/InputManager.hpp"
#include "../math/Mat4.hpp"
#include <imgui.h>

CreativeControls::CreativeControls(Camera* camera, float moveSpeed, float rotateSpeed) : CameraControls(camera), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed)
{
}

void CreativeControls::handleMouseMove(float xoffset, float yoffset)
{
    yaw += xoffset * rotateSpeed;
    pitch += yoffset * rotateSpeed;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    camera->transform.setRotation(Vec3(pitch, yaw, 0.0));
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
        camera->transform.translateLocal(Vec3(0, moveSpeed * Time::deltaTime(), 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::LSHIFT))
    {
        camera->transform.translateLocal(Vec3(0, -moveSpeed * Time::deltaTime(), 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::A))
    {
        camera->transform.translateLocal(Vec3(-moveSpeed * Time::deltaTime(), 0, 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::D))
    {
        camera->transform.translateLocal(Vec3(moveSpeed * Time::deltaTime(), 0, 0));
    }
    if (inputManager->isKeyPressed(KeyboardKey::W))
    {
        camera->transform.translateLocal(Vec3(0, 0, -moveSpeed * Time::deltaTime()));
    }
    if (inputManager->isKeyPressed(KeyboardKey::S))
    {
        camera->transform.translateLocal(Vec3(0, 0, moveSpeed * Time::deltaTime()));
    }
}