#include "WindowManager.hpp"
#include "../math/Vec4.hpp"
#include "../input/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager)
    {
        inputManager->handleMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

void scrollCallbaack(GLFWwindow* window, double xoffset, double yoffset)
{
    InputManager* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager)
    {
        inputManager->handleScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* WindowManager::getWindow() const
{
    return window;
}

unsigned int WindowManager::getWidth() const
{
    return width;
}

unsigned int WindowManager::getHeight() const
{
    return height;
}

void WindowManager::init(InputManager* inputManager, unsigned int width, unsigned int height, const char* name)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    this->width = width;
    this->height = height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);

    // Set the user pointer to the inputManager, used to to trigger callbacks on inputManager
    glfwSetWindowUserPointer(window, inputManager);

    glViewport(0, 0, width, height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallbaack);
}

void WindowManager::setTitle(const char* title)
{
    glfwSetWindowTitle(window, title);
}

void WindowManager::clear(Vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::checkForEvents()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        close();
    }
}

void WindowManager::close()
{
    glfwSetWindowShouldClose(window, true);
}

void WindowManager::update()
{
    checkForEvents();
    glfwSwapBuffers(window);
    glfwPollEvents();
}