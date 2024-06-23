#ifndef GUI_HPP
#define GUI_HPP

// Ensure GLEW is included before any other OpenGL headers
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <filesystem>
#include "../model/Model.hpp"
#include "../light/SceneLightInfo.hpp"


class GuiTransform 
{
public:
    float position[3];
    float rotation[3];
    float scale[3];

    Vec3 getPosition() const;
    Vec3 getRotation() const;
    Vec3 getScale() const;
    GuiTransform();
    GuiTransform(const Vec3 &pos, const Vec3 &rot, const Vec3 &scale);
};

class GuiModel
{
public:
    std::string name;
    GuiTransform transform;

    GuiModel(const std::string &name, GuiTransform transform);
};

class GUI
{
public:
    GUI();
    ~GUI();
    void init(GLFWwindow *mainWindow, std::vector<Model*>* app_models, SceneLightInfo* app_lightInfo); // Initialisation avec une fenêtre existante
    void render(); // Rendu de l'interface ImGui
    void cleanup(); // Nettoyage d'ImGui

private:
    GLFWwindow *window;
    int selected_index;
    std::vector<GuiModel> GUI_models;
    float GUI_lightColor[3];
    float GUI_lightIntensity;
    float GUI_volumetricLightIntensity;
    std::vector<Model*>* APP_models;
    SceneLightInfo* APP_lightInfo;
    void renderSceneWindow();
    void renderColorWindow();
    static void glfwErrorCallback(int error, const char *description);
    void updateTransform(const std::string &name, const GuiTransform &transform);
    void importAppData();
};

#endif
