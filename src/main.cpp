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
#include "wireframe/WireFrame.hpp"
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
#include "../postprocess/VolumetricFog.hpp"
#include "gui/Gui.hpp"

struct Application
{
public:
    WindowManager* windowManager;
    InputManager* inputManager;
    CreativeControls* cameraControls;

    Camera* cam;
    WireFrame grid;

    VolumetricFog volumetricFog;
    SceneLightInfo lightInfo;
    ShadowMap shadowMap;
    std::vector<Model *> models;

    int width = 800;
    int height = 600;
    GUI gui;

    void Init()
    {
        cam = new Camera(60, (float) width / height, 0.001f, 100.0f, Vec3(0, 0, 0));
        inputManager = new InputManager();
        windowManager = new WindowManager();
        cameraControls = new CreativeControls(cam, 10.0f, 0.2f);
        
        inputManager->init();
        inputManager->attachControls(cameraControls);
        windowManager->init(inputManager, width, height, "Holy Engine");

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

        ModelLoader loader;
        Model* atrium = new Model();
        loader.load("models/Atrium/atrium.obj", "models/Atrium", atrium->shapes, cam, &lightInfo, &shadowMap);
        atrium->transform = new Transform(Vec3(0.0, 0.0, 0), Vec3(0, 0, 0), Vec3(1.25, 1.25, 1.25));
        atrium->Init();
        models.push_back(atrium);
        atrium->name = "Atrium";

        Model* moai = new Model();
        loader.load("models/Moai/moai.obj", "models/Moai", moai->shapes, cam, &lightInfo, &shadowMap);
        moai->transform = new Transform(Vec3(0, -0.5f, 0), Vec3(0, 0, 0), Vec3(0.2, 0.2, 0.2));
        moai->Init();
        models.push_back(moai);
        moai->name = "Moaï";
        
        // Model* ancientTemple = new Model();
        // loader.load("models/AncientTemple/ancient-temple-stylized.obj", "models/AncientTemple", ancientTemple->shapes, cam, &lightInfo, &shadowMap);
        // ancientTemple->transform = new Transform(Vec3(0.0, 1.0, -32.0), Vec3(0, 180, 0), Vec3(0.5f, 0.5f, 0.5f));
        // ancientTemple->Init();
        // models.push_back(ancientTemple);
        // ancientTemple->name = "Ancient Temple";

        // Model* temple = new Model();
        // loader.load("models/EvoraTemple/34Million.obj", "models/EvoraTemple", temple->shapes, cam, &lightInfo, &shadowMap);
        // temple->transform = new Transform(Vec3(0, -3, 0), Vec3(-90, -40, 0), Vec3(1, 1, 1));
        // temple->Init();
        // models.push_back(temple);
        // temple->name = "Temple";

        // Init grid
        grid.GenerateGrid(20.0, 1.0);
        grid.Init();

        // Lighting
        lightInfo.lightColor = Vec3(1.0, 0.96, 0.71);
        lightInfo.lightPos = Vec3(0, 10, 0);
        lightInfo.ambientLight = Vec3(1, 1, 1) * 0.05;
        lightInfo.volumetricIntensity = 0.15f * 0;
        lightInfo.lightIntensity = 3.0f;
        
        // Shadow map
        // shadowMap.Create();
        shadowMap.Attach(&lightInfo);

        cam->transform.setPosition(Vec3(0, 5, 25));
        cam->transform.setRotation(Vec3(0, 0, 0));

        volumetricFog.Init(width, height);
        gui.init(windowManager->getWindow(), &models, &lightInfo);
    }

    void Update()
    {
        Time::update();
        // std::cout << "FPS: " << 1.0 / Time::deltaTime() << std::endl;
        inputManager->update(windowManager->getWindow());
        cameraControls->update(inputManager);

        // Move light
        float time = Time::time();
        float speed = 0.1f;
        float radius = 20.0f;
        float lightX = radius * cos(time * speed);
        float lightZ = radius * sin(time * speed);
        float lightY = 20.0f;
        lightInfo.lightPos = Vec3(lightX, lightY, lightZ);

        if (inputManager->isKeyPressed(KeyboardKey::Escape))
        {
            Terminate();
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
		windowManager->clear(Vec4(1.0, 0, 0, 1.0));
        // shadowMap.Compute(models);
        glViewport(0, 0, width, height);
        
        // Bind the custom framebuffer (FBO)
        glBindFramebuffer(GL_FRAMEBUFFER, volumetricFog.FBO);
		windowManager->clear(Vec4(lightInfo.lightColor.x, lightInfo.lightColor.y, lightInfo.lightColor.z, 1.0));

        // Draw scene to FBO
        grid.Draw(*cam);
        for (std::size_t i = 0; i < models.size(); i++)
        {
            models[i]->Draw(cam);
        }

        // Render to window with post process
        volumetricFog.Render(&shadowMap, cam, &lightInfo);
        gui.render();

        glfwSwapBuffers(windowManager->getWindow());
        glfwPollEvents();
    }

    void Terminate()
    {
        // Deallocate all resources
        grid.Release();
        volumetricFog.Release();
        shadowMap.Release();
        for (Model* model : models)
        {
            model->Release();
            delete model;
            model = nullptr;
        }
        models.clear();
        delete windowManager;
        delete inputManager;
        delete cameraControls;
        delete cam;

        gui.cleanup();
        glfwTerminate();
        exit(0);
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