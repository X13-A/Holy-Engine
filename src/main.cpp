#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


#include "../common/GLShader.h"
#include "mesh_loader.hpp"
#include "geometry/Vertex.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"
#include "geometry/Mesh.hpp"

// g++ -o main main.cpp ./common/GLShader.cpp mesh_loader.cpp -lglut -lGL -lGLU -lglfw -lGLEW && ./main

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

        stbi_set_flip_vertically_on_load(true);
        mesh.Init();
    }

    void Render()
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

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
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;

    MeshLoader loader;
    loader.load("models/Cat.obj", vertices, normals, uvs);
    std::cout << vertices.size() << std::endl;
    std::cout << normals.size() << std::endl;
    std::cout << uvs.size() << std::endl;

    while (!glfwWindowShouldClose(app.window))
    {
        app.Render();
    }


    app.Terminate();

    return 0;
}