#include "CreativeControls.hpp"
#include "../input/InputManager.hpp"
#include "../math/Mat4.hpp"

CreativeControls::CreativeControls(Camera* camera, float moveSpeed, float rotateSpeed) : CameraControls(camera), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed)
{
}

void CreativeControls::handleMouseMove(float xoffset, float yoffset)
{
    yaw += xoffset * rotateSpeed;
    pitch += yoffset * rotateSpeed;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    camera->transform.setPitchYaw(pitch, yaw);
    // camera->transform.setPitchYaw(pitch, yaw); // TODO: Fix this method

    // std::cout << "xOffset: " << xoffset << ", yOffset: " << yoffset << std::endl;
    // std::cout << "FPS: " << 1.0f / Time::deltaTime() << std::endl;
    // std::cout << "yaw: " << yaw << ", pitch: " << pitch << std::endl;

    // Vec3 direction;
    // direction.x = -cos(radians(yaw)) * cos(radians(pitch));
    // direction.y = sin(radians(pitch));
    // direction.z = sin(radians(yaw)) * cos(radians(pitch));
    // direction = Vec3::normalize(direction);
    // direction.printValues();
    // camera->transform.getPosition().printValues();
    // Vec3 right = Vec3::normalize(Vec3::cross(Vec3(0.0f, 1.0f, 0.0f), direction));
    // Vec3 up = Vec3::normalize(Vec3::cross(direction, right));
    // camera->transform.lookat(camera->transform.getPosition() + direction, Vec3(0, 1, 0));
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