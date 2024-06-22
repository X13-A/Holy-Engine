#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"
#include "common/GLShader.h"
#include "geometry/Vertex.hpp"
#include "math/Vec3.hpp"
#include "math/Vec2.hpp"
#include "geometry/Mesh.hpp"
#include "grid/Grid.hpp"
#include "math/Mat4.hpp"
#include "camera/Camera.hpp"
#include "geometry/Transform.hpp"
#include "window/WindowManager.hpp"
#include "input/InputManager.hpp"
#include "controls/CreativeControls.hpp"
#include "geometry/ModelLoader.hpp"
#include "model/Model.hpp"
#include "materials/Material.hpp"
#include "materials/LitMaterial.hpp"
#include "light/SceneLightInfo.hpp"
#include "light/ShadowMap.hpp"

const float screenVertices[6 * 4] =
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

struct Application
{
public:
    WindowManager* windowManager;
    InputManager* inputManager;
    CreativeControls* cameraControls;

    Camera* cam;
    WireFrame grid;

    SceneLightInfo lightInfo;
    ShadowMap shadowMap;
    std::vector<Model *> models;

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
        
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        stbi_set_flip_vertically_on_load(true);

        Model* atrium = new Model();
        ModelLoader loader;
        loader.load("models/Atrium/atrium.obj", "models/Atrium", atrium->shapes, cam, &lightInfo, &shadowMap);
        atrium->transform = new Transform(Vec3(0, 0.0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1));
        atrium->Init();
        models.push_back(atrium);

        Model* rafale = new Model();
        loader.load("models/Rafale/Rafale.obj", "models/Rafale", rafale->shapes, cam, &lightInfo, &shadowMap);
        rafale->transform = new Transform(Vec3(0, 3.0, 0), Vec3(0, 90, 0), Vec3(0.05, 0.05, 0.05));
        rafale->Init();
        models.push_back(rafale);

        // Init grid
        grid.GenerateGrid(20.0, 1.0);
        grid.Init();

        // Lighting
        lightInfo.lightColor = Vec3(1.0, 0.96, 0.71) * 3;
        lightInfo.lightPos = Vec3(0, 10, 0);
        lightInfo.ambientLight = Vec3(1, 1, 1) * 0.05;

        shadowMap.Create();
        shadowMap.Attach(&lightInfo);

        cam->transform.setPosition(Vec3(0, 20, 0));
        cam->transform.setRotation(Vec3(-90, 0, 0));
    }

    void Update()
    {
        Time::update();
        inputManager->update(windowManager->getWindow());
        cameraControls->update(inputManager);

        // Move light
        float time = Time::time();
        float radius = 20.0f;
        float lightX = radius * cos(time);
        float lightZ = radius * sin(time);
        float lightY = 20.0f;
        lightInfo.lightPos = Vec3(lightX, lightY, lightZ);

        models[1]->transform->setRotation(Vec3(0, Time::time() * 1, 0));

        if (inputManager->isKeyPressed(KeyboardKey::Escape))
        {
            Terminate();
            exit(0);
        }

        // Ensure consistent calculations with matrices
        Vec3 pos = cam->transform.getPosition();
        Vec3 matrixPos = cam->transform.getTransformMatrixPosition();
        float delta = Vec3::distance(pos, matrixPos);
        if (delta > 0)
        {
            std::cout << "WARNING: Wrong calculation of transformation matrix (delta: " << delta << ")" << std::endl;
        }
    }

    void Render()
    {
        /* Render here */
        windowManager->clear(Vec4(1.0, 0.99, 0.90, 1.0));
        grid.Draw(*cam);
        shadowMap.Compute(models);
        glViewport(0, 0, width, height);
        
        // Quick hack to check shadowmap
        // models[0]->shapes[0]->material->albedoTexID = shadowMap.GetDepthMap();
        
        for (std::size_t i = 0; i < models.size(); i++)
        {
            models[i]->Draw(cam);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManager->getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    void Terminate()
    {
        // TODO: Deallocate resources
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