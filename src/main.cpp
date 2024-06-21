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
#include "grid/Grid.hpp"
#include "math/Mat4.hpp"
#include "camera/Camera.hpp"
#include "geometry/Transform.hpp"
#include "window/WindowManager.hpp"
#include "input/InputManager.hpp"
#include "controls/CreativeControls.hpp"

#include "model/Model.hpp"
#include "materials/Material.hpp"
#include "materials/LitMaterial.hpp"
#include "light/SceneLightInfo.hpp"

Model* CreateModel(const char* meshPath, Material* material)
{
    Transform* transform = new Transform(Vec3(0, 0, 0), Vec3(), Vec3(1, 1, 1));
    
    Mesh* mesh = new Mesh();
    mesh->Load(meshPath);
    mesh->Init();

    Model *model = new Model();
    model->transform = transform;
    model->mesh = mesh;
    model->material = material;
    model->Init();
    return model;
}

struct Application
{
public:
    WindowManager* windowManager;
    InputManager* inputManager;
    CreativeControls* cameraControls;

    Camera* cam;
    WireFrame grid;

    SceneLightInfo lightInfo;
    std::vector<Model*> models;

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

        LitMaterial *rafaleMaterial = new LitMaterial();
        rafaleMaterial->Init("./shaders/vert.glsl", "./shaders/frag.glsl");
        rafaleMaterial->Attach(cam, &lightInfo);
        rafaleMaterial->metallic = 0.0f;
        rafaleMaterial->smoothness = 0.9f;
        Model *rafale = CreateModel("./models/Rafale.obj", rafaleMaterial);
        models.push_back(rafale);

        LitMaterial *catMaterial = new LitMaterial();
        catMaterial->Init("./shaders/vert.glsl", "./shaders/frag.glsl");
        catMaterial->Attach(cam, &lightInfo);
        catMaterial->metallic = 1.0f;
        catMaterial->smoothness = 0.9f;
        Model *cat = CreateModel("./models/Cat.obj", catMaterial);
        models.push_back(cat);
        cat->transform->setPosition(Vec3(10, 0, 0));
        cat->transform->setScale(Vec3(0.2, 0.2, 0.2));
        // Init grid
        grid.GenerateGrid(20.0, 1.0);
        grid.Init();

        // Lighting
        lightInfo.lightColor = Vec3(5, 5, 5);
        lightInfo.lightPos = Vec3(0, 10, 0);
        lightInfo.ambientLight = Vec3(1, 1, 1) * 0.05;

        cam->transform.setPosition(Vec3(0, 0, 10));
    }

    void Update()
    {
        Time::update();
        inputManager->update(windowManager->getWindow());
        cameraControls->update(inputManager);

        // Move light
        float time = Time::time();
        float radius = 10.0f;
        float lightX = radius * cos(time);
        float lightZ = radius * sin(time);
        float lightY = 5.0f;
        lightInfo.lightPos = Vec3(lightX, lightY, lightZ);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        grid.Draw(*cam);
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