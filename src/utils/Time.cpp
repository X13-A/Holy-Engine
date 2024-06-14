#include "Time.hpp"
#include "GLFW/glfw3.h"

double Time::lastTime = 0;
double Time::_deltaTime = 0;

double Time::deltaTime()
{
    return Time::_deltaTime;
}

double Time::time()
{
    return glfwGetTime();
}

void Time::update() 
{
    double currentTime = Time::time();
    _deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}