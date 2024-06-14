#include "InputManager.hpp"
#include "../math/Vec2.hpp"
#include <iostream>

InputManager::InputManager()
{
	lastMousePos = Vec2(0, 0);
}

void InputManager::init()
{
	keysPressed.resize(static_cast<size_t>(KeyboardKey::KeysCount), false);
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

	std::cout << xpos << ", " << ypos << std::endl;
	// if (cameraControls == nullptr) return;

	// float offsetX = lastMousePos.x - xpos;
	// float offsetY = lastMousePos.y - ypos;

	lastMousePos.x = xpos;
	lastMousePos.y = ypos;

	// cameraControls->handleMouseMove(offsetX, offsetY);
}

void InputManager::handleScroll(float xoffset, float yoffset)
{
	std::cout << xoffset << ", " << yoffset << std::endl;
	// cameraControls->handleScroll(xoffset, yoffset);
}

bool InputManager::isKeyPressed(KeyboardKey key)
{
	return keysPressed.at(static_cast<size_t>(key));
}

void InputManager::update(GLFWwindow* window)
{
	retrieveInputs(window);
}