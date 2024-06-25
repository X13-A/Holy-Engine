#include "Gui.hpp"
#include <iostream>
#include <fstream>
#include "../math/Vec4.hpp"
#include "../math/Vec3.hpp"
#include "../utils/Time.hpp"
#include <string>

// Implémentation de la classe GuiTransform
GuiTransform::GuiTransform(const Vec3& pos, const Vec3& rot, const Vec3& scale)
{
    this->position[0] = pos.x;
    this->position[1] = pos.y;
    this->position[2] = pos.z;

    this->rotation[0] = rot.x;
    this->rotation[1] = rot.y;
    this->rotation[2] = rot.z;

    this->scale[0] = scale.x;
    this->scale[1] = scale.y;
    this->scale[2] = scale.z;
}

Vec3 GuiTransform::getPosition() const
{
    return Vec3(position[0], position[1], position[2]);
}

Vec3 GuiTransform::getRotation() const
{
    return Vec3(rotation[0], rotation[1], rotation[2]);
}

Vec3 GuiTransform::getScale() const
{
    return Vec3(scale[0], scale[1], scale[2]);
}

// Implémentation de la classe GuiModel
GuiModel::GuiModel(const std::string &name, GuiTransform transform) : name(name), transform(transform) 
{
}

GUI::GUI() : window(nullptr), selected_index(-1), GUI_lightColor{0.45f, 0.55f, 0.60f}, GUI_lightIntensity(1.0f)
{
}

GUI::~GUI()
{
}

void GUI::importAppData()
{
    GUI_models.clear();
    for (Model *model : *APP_models)
    {
        selected_index = 0; // Select first by default
        GuiTransform guiTransform = GuiTransform(model->transform->getPosition(), model->transform->getRotation(), model->transform->getScale());
        GuiModel guiModel = GuiModel(model->name, guiTransform);
        GUI_models.push_back(guiModel);
    }
    GUI_lightColor[0] = APP_lightInfo->lightColor.x;
    GUI_lightColor[1] = APP_lightInfo->lightColor.y;
    GUI_lightColor[2] = APP_lightInfo->lightColor.z;
    GUI_lightIntensity = APP_lightInfo->lightIntensity;
    GUI_volumetricLightIntensity = APP_lightInfo->volumetricIntensity;
}

void GUI::init(GLFWwindow *mainWindow, std::vector<Model*>* app_models, SceneLightInfo* app_lightInfo)
{
    APP_models = app_models;
    APP_lightInfo = app_lightInfo;
    window = mainWindow;
    glfwSetErrorCallback(glfwErrorCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    importAppData();
}

void GUI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderSceneWindow();
    renderColorWindow();
    renderMetrics();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::renderMetrics()
{
    ImGui::Begin("Performance");
    ImGui::Text((std::to_string(Time::FPS()) + " FPS").c_str());
    ImGui::End();
}

void GUI::renderSceneWindow()
{
    ImGui::Begin("Scene models");

    for (size_t i = 0; i < GUI_models.size(); ++i)
    {
        ImGui::PushID(i);
        ImGui::SameLine();
        std::string label = GUI_models[i].name + " (" + std::to_string((*APP_models)[i]->shapes.size()) + " meshes)";
        if (ImGui::Button(label.c_str()));
        {
            selected_index = i;
        }
        ImGui::PopID();
    }

    if (!GUI_models.empty() && selected_index != -1 && selected_index < GUI_models.size()) 
    {
        ImGui::Text("Selected: %s", GUI_models[selected_index].name.c_str());
        ImGui::Separator();
        ImGui::Text("Position:");
        ImGui::InputFloat3("Position", GUI_models[selected_index].transform.position);
        ImGui::Text("Rotation:");
        ImGui::InputFloat3("Rotation", GUI_models[selected_index].transform.rotation);
        ImGui::Text("Scale:");
        ImGui::InputFloat3("Scale", GUI_models[selected_index].transform.scale);
        if (ImGui::Button("Apply")) 
        {
            updateTransform(GUI_models[selected_index].name, GUI_models[selected_index].transform);
        }
    }

    ImGui::End();
}

void GUI::renderColorWindow() 
{
    ImGui::Begin("Lighting settings");
    ImGui::ColorEdit3("Light color", GUI_lightColor);
    ImGui::SliderFloat("Light intensity", &GUI_lightIntensity, 0.0f, 5.0f);
    ImGui::SliderFloat("Volumetrics intensity", &GUI_volumetricLightIntensity, 0.0f, 1.0f);
    APP_lightInfo->volumetricIntensity = GUI_volumetricLightIntensity;
    APP_lightInfo->lightIntensity = GUI_lightIntensity;
    APP_lightInfo->lightColor = Vec3(GUI_lightColor[0], GUI_lightColor[1], GUI_lightColor[2]);
    ImGui::End();
}

void GUI::glfwErrorCallback(int error, const char *description) 
{
    fprintf(stderr, "Erreur GLFW %d: %s\n", error, description);
}

void GUI::updateTransform(const std::string &name, const GuiTransform &transform) 
{
    (*APP_models)[selected_index]->transform->setPosition(transform.getPosition());
    (*APP_models)[selected_index]->transform->setRotation(transform.getRotation());
    (*APP_models)[selected_index]->transform->setScale(transform.getScale());
    std::cout << name << ":\n"
              << "  Position: (" << transform.position[0] << ", " << transform.position[1] << ", " << transform.position[2] << ")\n"
              << "  Rotation: (" << transform.rotation[0] << ", " << transform.rotation[1] << ", " << transform.rotation[2] << ")\n"
              << "  Scale: (" << transform.scale[0] << ", " << transform.scale[1] << ", " << transform.scale[2] << ")\n";
}