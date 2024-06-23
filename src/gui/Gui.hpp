#ifndef GUI_HPP
#define GUI_HPP

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <filesystem>

// Classe GuiTransform
class GuiTransform {
public:
    float position[3];
    float rotation[3];
    float scale[3];

    GuiTransform();
};

// Classe Object
class Object {
public:
    std::string name;
    GuiTransform transform;
    char folderPath[256];

    Object(const std::string &name, const std::string &path);
};

// Classe GUI
class GUI {
public:
    GUI();
    ~GUI();
    void run();

private:
    GLFWwindow *window;
    std::vector<Object> objects;
    int selected_index;
    float color[3];
    float intensity;

    void init();
    void mainLoop();
    void renderSceneWindow();
    void renderColorWindow();
    void cleanup();
    static void glfwErrorCallback(int error, const char *description);
    void printTransform(const std::string &name, const GuiTransform &transform);
    void copyDirectory(const std::filesystem::path &source, const std::filesystem::path &destination);
    std::vector<Object> loadObjectsFromDirectory(const std::string &directory);
};

#endif // GUI_HPP
