#include "gui.hpp"
#include <iostream>

Gui::Gui() : showMenu(false), selectedIndex(0), selectedOption("") {
    options = {"Option 1", "Option 2", "Option 3"};
}

Gui::~Gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Enable keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void Gui::render() {
    if (showMenu) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::BeginMenu("Options")) {
            for (int i = 0; i < options.size(); ++i) {
                if (ImGui::MenuItem(options[i].c_str(), NULL, selectedIndex == i)) {
                    selectedIndex = i;
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::Button("Select")) {
            selectedOption = options[selectedIndex];
            std::cout << "Selected: " << selectedOption << std::endl;
            showMenu = false;  // Close menu after selection
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void Gui::handleInput(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        showMenu = !showMenu;
        if (!showMenu) {
            std::cout << "Menu closed. Last selected: " << getSelectedOption() << std::endl;
        }
    }
}

std::string Gui::getSelectedOption() const {
    return selectedOption;
}
