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
#include "controls/CreativeControls.hpp"

struct Application
{
public:
    WindowManager* windowManager;
    InputManager* inputManager;
    CreativeControls* cameraControls;

    Mesh mesh;
    Transform* transform;
    Camera* cam;
    

    int width = 640;
    int height = 640;

    void Init()
    {
        cam = new Camera(90, width / height, 0.1f, 100.0f);
        transform = new Transform(Vec3(), Vec3(), Vec3(1, 1, 1));

        inputManager = new InputManager();
        windowManager = new WindowManager();
        cameraControls = new CreativeControls(cam, 10.0f, 0.2f);
        
        inputManager->init();
        inputManager->attachControls(cameraControls);
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

        mesh.Load("models/Rafale.obj");
        mesh.Init();
    }

    void Update()
    {
        Time::update();
        float rot = Time::time() * 1;
        transform->setPosition(Vec3(sin(rot) * 0, 0, -10));
        // cam->transform.setPosition(Vec3(cos(rot) * 5, sin(rot) * 10, 0));

        inputManager->update(windowManager->getWindow());
        cameraControls->update(inputManager);
        // cam->transform.lookat(transform->getPosition());
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