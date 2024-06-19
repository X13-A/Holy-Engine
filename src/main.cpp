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

    std::vector<Mesh> meshes;
    std::vector<Transform*> transforms;
    Camera* cam;

    int width = 640;
    int height = 640;

    void Init()
    {
        cam = new Camera(60, width / height, 0.001f, 100.0f, Vec3(0, 0, 0));
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
        glCullFace(GL_FRONT);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        stbi_set_flip_vertically_on_load(true);

        // Init meshes
        Mesh mesh1;
        mesh1.Load("models/Rafale.obj");
        mesh1.Init();
        meshes.push_back(mesh1);
        Transform* transform1 = new Transform(Vec3(0, 0, -15), Vec3(), Vec3(1, 1, 1));
        transforms.push_back(transform1);

        // Mesh mesh2;
        // mesh2.Load("models/Box.obj");
        // mesh2.Init();
        // meshes.push_back(mesh2);
        // Transform* transform2 = new Transform(Vec3(0, 0, 0), Vec3(), Vec3(10, 0.1, 10));
        // transforms.push_back(transform2);

        // Mat4 transformMatrix;
        cam->transform.setPosition(Vec3(0, 0, 0));

    }

    void Update()
    {
        Time::update();
        inputManager->update(windowManager->getWindow());
        cameraControls->update(inputManager);

        Vec3 pos = cam->transform.getPosition();
        Vec3 matrixPos = cam->transform.getTransformMatrixPosition();
        float delta = Vec3::distance(pos, matrixPos);
        if (delta > 0)
        {
            std::cout << "ERROR: Wrong calculation of transformation matrix (delta: " << delta << ")" << std::endl;
        }
        pos.printValues();
    }

    void Render()
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(*transforms[i], *cam);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManager->getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    void Terminate()
    {
        // Deallocate resources
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Destroy();
            free(transforms[i]);
        }
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