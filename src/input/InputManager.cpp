#include "InputManager.hpp"
#include "../math/Vec2.hpp"
#include <iostream>
#include "../controls/CameraControls.hpp"

InputManager::InputManager()
{
	lastMousePos = Vec2(0, 0);
}

void InputManager::init()
{
	keysPressed.resize(static_cast<size_t>(KeyboardKey::KeysCount), false);
}

void InputManager::attachControls(CameraControls* cameraControls)
{
	this->cameraControls = cameraControls;
}

void InputManager::retrieveInputs(GLFWwindow* window)
{
	for (const std::pair<int, KeyboardKey> pair : keyMap)
	{
		if (pair.second == KeyboardKey::KeysCount) break;
		bool keyPressed = glfwGetKey(window, pair.first) == GLFW_PRESS;
		keysPressed.at(static_cast<size_t>(pair.second)) = keyPressed;
	}
}

void InputManager::handleMouseMovement(float xpos, float ypos)
{
	if (firstMouseInput)
	{
		firstMouseInput = false;
		lastMousePos.x = xpos;
		lastMousePos.y = ypos;
	}

	if ((Vec3(xpos, ypos, 0) - Vec3(lastMousePos.x, lastMousePos.y, 0)).length() > 100)
	{
		lastMousePos.x = xpos;
		lastMousePos.y = ypos;
		return;
	}
	if (cameraControls == nullptr) return;

	float offsetX = lastMousePos.x - xpos;
	float offsetY = lastMousePos.y - ypos;


	lastMousePos.x = xpos;
	lastMousePos.y = ypos;

	cameraControls->handleMouseMove(offsetX, offsetY);
}

void InputManager::handleScroll(float xoffset, float yoffset)
{
	cameraControls->handleScroll(xoffset, yoffset);
}

bool InputManager::isKeyPressed(KeyboardKey key)
{
	return keysPressed.at(static_cast<size_t>(key));
}

void InputManager::update(GLFWwindow* window)
{
	retrieveInputs(window);
}