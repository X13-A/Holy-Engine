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

// Model* CreateModel(const char* meshPath, Material* material)
// {
//     Transform* transform = new Transform(Vec3(0, 0, 0), Vec3(), Vec3(1, 1, 1));
    
//     Mesh* mesh = new Mesh();
//     mesh->Load(meshPath);
//     mesh->Init();

//     Model *model = new Model();
//     model->transform = transform;
//     model->mesh = mesh;
//     model->material = material;
//     model->Init();
//     return model;
// }

float rectangleVertices[] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
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
    std::vector<Model*> models;

    int width = 640;
    int height = 640;

    unsigned int FBO;
    unsigned int framebufferTexture;
    unsigned int RBO;
    unsigned int rectVAO, rectVBO;

    GLShader framebufferShader;
    uint32_t framebufferProgram;

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
        loader.load("models/Atrium/atrium.obj", "models/Atrium", atrium->shapes, cam, &lightInfo);
        atrium->transform = new Transform(Vec3(0, 0.0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1));
        atrium->Init();
        models.push_back(atrium);

        Model* rafale = new Model();
        loader.load("models/Rafale/Rafale.obj", "models/Rafale", rafale->shapes, cam, &lightInfo);
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

        cam->transform.setPosition(Vec3(0, 20, 0));
        cam->transform.setRotation(Vec3(-90, 0, 0));

        //framebufferProgram.Activate();
        framebufferShader.LoadVertexShader("shaders/framebuffer.vert.glsl");
        framebufferShader.LoadFragmentShader("shaders/framebuffer.frag.glsl");
        framebufferShader.Create();

        framebufferProgram = framebufferShader.GetProgram();
        glUseProgram(framebufferProgram);
        glUniform1i(glGetUniformLocation(framebufferProgram, "ScreenTexture"),0);


        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        
        // Create Frame Buffer Object
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create Framebuffer Texture
        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        // Create Render Buffer Object
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


        // Error checking framebuffer
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer error: " << fboStatus << std::endl;

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
        // Bind the custom framebuffer (FBO)
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // Specify the color of the background
		windowManager->clear(Vec4(1.0, 0.99, 0.90, 1.0));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Dessiner la scène vers le FBO
        grid.Draw(*cam);
        for (std::size_t i = 0; i < models.size(); i++) {
            models[i]->Draw(cam);
        }

        // Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Utiliser le shader d'inversion des couleurs et dessiner un rectangle couvrant l'écran
        glUseProgram(framebufferProgram);
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(windowManager->getWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }

    void Terminate()
    {
        framebufferShader.Destroy();
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