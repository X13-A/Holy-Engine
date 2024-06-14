#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "../math/Vec4.hpp"
#include <GLFW/glfw3.h>
#include "../input/InputManager.hpp"

class WindowManager
{
    private:
        unsigned int width;
        unsigned int height;

    public:
        GLFWwindow* window;

        GLFWwindow* getWindow() const;

        unsigned int getWidth() const;

        unsigned int getHeight() const;

        void init(InputManager* inputManager, unsigned int width, unsigned int height, const char* name);

        void setTitle(const char* title);

        void clear(Vec4 color);

        void checkForEvents();

        void close();

        void update();
};

#endif