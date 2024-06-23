#ifndef GUI_HPP
#define GUI_HPP

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Gui {
public:
    Gui();
    ~Gui();

    void initialize(GLFWwindow* window);
    void render();
    void handleInput(int key, int scancode, int action, int mods);
    std::string getSelectedOption() const;

private:
    bool showMenu;
    std::vector<std::string> options;
    int selectedIndex;
    std::string selectedOption;
};

#endif // GUI_HPP
