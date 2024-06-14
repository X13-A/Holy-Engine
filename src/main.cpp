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
#include "camera/Camera.hpp"
#include "geometry/Transform.hpp"
#include "window/WindowManager.hpp"
#include "input/InputManager.hpp"

struct Application
{
public:
    WindowManager* windowManager;
    InputManager* inputManager;

    Mesh mesh;
    Transform* transform;
    Camera* cam;
    

    int width = 640;
    int height = 640;

    void Init()
    {
        inputManager = new InputManager();
        windowManager = new WindowManager();

        inputManager->init();
        windowManager->init(inputManager, width, height, "Hello Engine");

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


        cam = new Camera(90, width / height, 0.1f, 100.0f);
        transform = new Transform(Vec3(), Vec3(), Vec3(1, 1, 1));

        mesh.Load("models/Rafale.obj");
        mesh.Init();
    }

    void Update()
    {
        float x = sin(glfwGetTime()) * 10.0f;
        transform->setPosition(Vec3(x, 0, -5));
        cam->transform.setPosition(Vec3(-x, 0, 0));
        cam->transform.lookat(transform->getPosition());
    }

    void Render()
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.Draw(*transform, *cam);

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManager->getWindow());

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

    while (!glfwWindowShouldClose(app.windowManager->getWindow()))
    {
        app.Update();
        app.Render();
    }

    app.Terminate();

    return 0;
}