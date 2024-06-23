#include "Gui.hpp"
#include <iostream>
#include <fstream>

// Implémentation de la classe GuiTransform
GuiTransform::GuiTransform() {
    position[0] = position[1] = position[2] = 0.0f;
    rotation[0] = rotation[1] = rotation[2] = 0.0f;
    scale[0] = scale[1] = scale[2] = 1.0f;
}

// Implémentation de la classe Object
Object::Object(const std::string &name, const std::string &path) : name(name) {
    strncpy(folderPath, path.c_str(), sizeof(folderPath));
    folderPath[sizeof(folderPath) - 1] = '\0'; // Ensure null termination
}

// Implémentation de la classe GUI
GUI::GUI() : window(nullptr), selected_index(-1), color{0.45f, 0.55f, 0.60f}, intensity(1.0f) {
    init();
}

GUI::~GUI() {
    cleanup();
}

void GUI::init() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    const char *glsl_version = "#version 130";
    window = glfwCreateWindow(1280, 720, "ImGui + GLFW + OpenGL3", NULL, NULL);
    if (window == NULL)
        throw std::runtime_error("Failed to create GLFW window");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    objects = loadObjectsFromDirectory("models");
}

void GUI::run() {
    mainLoop();
}

void GUI::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderSceneWindow();
        renderColorWindow();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GUI::renderSceneWindow() {
    ImGui::Begin("Scène");

    for (size_t i = 0; i < objects.size(); ++i) {
        ImGui::PushID(i);
        if (ImGui::Button("Supprimer")) {
            objects.erase(objects.begin() + i);
            if (selected_index == i) {
                selected_index = -1;
            } else if (selected_index > i) {
                selected_index--;
            }
            ImGui::PopID();
            continue;
        }
        ImGui::SameLine();
        if (ImGui::Button(objects[i].name.c_str())) {
            selected_index = i;
        }
        ImGui::PopID();
    }

    if (ImGui::Button("Ajouter")) {
        objects.push_back(Object("Nouvelle Option", ""));
    }

    if (!objects.empty() && selected_index != -1 && selected_index < objects.size()) {
        ImGui::Text("Vous avez choisi : %s", objects[selected_index].name.c_str());
        ImGui::InputText("Nom de l'objet", &objects[selected_index].name[0], objects[selected_index].name.size() + 1);
        ImGui::Separator();
        ImGui::Text("Position:");
        ImGui::InputFloat3("Position", objects[selected_index].transform.position);
        ImGui::Text("Rotation:");
        ImGui::InputFloat3("Rotation", objects[selected_index].transform.rotation);
        ImGui::Text("Scale:");
        ImGui::InputFloat3("Scale", objects[selected_index].transform.scale);
        ImGui::Text("Chemin du dossier:");
        ImGui::InputText("Chemin du dossier", objects[selected_index].folderPath, IM_ARRAYSIZE(objects[selected_index].folderPath));
        if (ImGui::Button("Appliquer")) {
            printTransform(objects[selected_index].name, objects[selected_index].transform);
            if (strlen(objects[selected_index].folderPath) > 0) {
                std::string destinationPath = "models/" + std::filesystem::path(objects[selected_index].folderPath).filename().string();
                copyDirectory(objects[selected_index].folderPath, destinationPath);
                std::cout << "Dossier copié à : " << destinationPath << std::endl;
            }
        }
    }

    ImGui::End();
}

void GUI::renderColorWindow() {
    ImGui::Begin("Réglage de la couleur et de l'intensité");
    ImGui::ColorEdit3("Couleur", color);
    ImGui::SliderFloat("Intensité", &intensity, 0.0f, 2.0f);
    ImGui::End();
}

void GUI::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void GUI::glfwErrorCallback(int error, const char *description) {
    fprintf(stderr, "Erreur GLFW %d: %s\n", error, description);
}

void GUI::printTransform(const std::string &name, const GuiTransform &transform) {
    // std::cout << name << ":\n"
    //           << "  Position: (" << transform.position[0] << ", " << transform.position[1] << ", " << transform.position[2] << ")\n"
    //           << "  Rotation: (" << transform.rotation[0] << ", transform.rotation[1] << ", " << transform.rotation[2] << ) << "\n"
    //           << "  Scale: (" << transform.scale[0] << ", " << transform.scale[1] << ", " << transform.scale[2] << ")\n";
}

void GUI::copyDirectory(const std::filesystem::path &source, const std::filesystem::path &destination) {
    try {
        std::filesystem::create_directories(destination);
        for (const auto &entry : std::filesystem::recursive_directory_iterator(source)) {
            const auto &path = entry.path();
            auto relativePath = std::filesystem::relative(path, source);
            std::filesystem::copy(path, destination / relativePath, std::filesystem::copy_options::overwrite_existing);
        }
    } catch (std::filesystem::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::vector<Object> GUI::loadObjectsFromDirectory(const std::string &directory) {
    std::vector<Object> objects;
    for (const auto &entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_directory()) {
            std::string folderName = entry.path().filename().string();
            std::string folderPath = entry.path().string();
            objects.emplace_back(folderName, folderPath);
        }
    }
    return objects;
}

