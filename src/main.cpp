#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"
#include "common/GLShader.h"
#include "geometry/mesh_loader.hpp"
#include "geometry/Vertex.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"
#include "geometry/Mesh.hpp"
#include "math/Mat4.hpp"

// g++ -o main src/main.cpp common/GLShader.cpp ./src/mesh_loader.cpp -lglut -lGL -lGLU -lglfw -lGLEW && ./main

struct Application
{
public:
    GLFWwindow *window;
    Mesh mesh;

    void Init()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
        if (!window)
        {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            exit(EXIT_FAILURE);
        }

        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        stbi_set_flip_vertically_on_load(true);

        mesh.Load("models/Rafale.obj");
        mesh.Init();
    }

    void Render()
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.Draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    void Terminate()
    {
        // Deallocate resources
        mesh.Destroy();
        glfwTerminate();
    }
};

int main(void)
{
    Application app;
    app.Init();

    while (!glfwWindowShouldClose(app.window))
    {
        app.Render();
    }

    app.Terminate();

    return 0;
}