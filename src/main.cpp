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

    unsigned int FBO;
    unsigned int framebufferTexture;
    unsigned int RBO;
    unsigned int rectVAO, rectVBO;

    GLShader framebufferShader;
    uint32_t framebufferProgram;
    GLuint depthTexture;

    void Init()
    {
        cam = new Camera(60, width / height, 0.001f, 100.0f, Vec3(0, 0, 0));
        inputManager = new InputManager();
        windowManager = new WindowManager();
        cameraControls = new CreativeControls(cam, 10.0f, 0.2f);
        
        inputManager->init();
        inputManager->attachControls(cameraControls);
        windowManager->init(inputManager, width, height, "Temple Engine");

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

        cam->transform.setPosition(Vec3(-5, 5, -5));
        cam->transform.setRotation(Vec3(-90, 0, 0));

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

        // Create depth texture
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);


        // Create Render Buffer Object
        // glGenRenderbuffers(1, &RBO);
        // glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        // Error checking framebuffer
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Framebuffer error: " << fboStatus << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  

        // Post process shader
        framebufferShader.LoadVertexShader("shaders/postprocess/framebuffer.vert.glsl");
        framebufferShader.LoadFragmentShader("shaders/postprocess/framebuffer.frag.glsl");
        framebufferShader.Create();
        framebufferProgram = framebufferShader.GetProgram();
        
        // Set screen texture
        glUseProgram(framebufferProgram);
        glUniform1i(glGetUniformLocation(framebufferProgram, "ScreenTexture"), 0);
        glUniform1i(glGetUniformLocation(framebufferProgram, "DepthTexture"), 1);
        glUseProgram(0);

        // Post process VAO
        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
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
        lightInfo.lightPos = Vec3(10, lightY, 10);

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
		windowManager->clear(Vec4(1.0, 0, 0, 1.0));
        shadowMap.Compute(models);
        glViewport(0, 0, width, height);
        
        // Bind the custom framebuffer (FBO)
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		windowManager->clear(Vec4(1.0, 0.99, 0.90, 1.0));

        // Draw scene to FBO
        grid.Draw(*cam);
        for (std::size_t i = 0; i < models.size(); i++)
        {
            models[i]->Draw(cam);
        }

        // Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Draw FBO color with post process shader
        glUseProgram(framebufferProgram);
		glBindVertexArray(rectVAO);
        
        // Bind screen texture
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glUniform1i(glGetUniformLocation(framebufferProgram, "ScreenTexture"), 0);

        // Bind depth texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glUniform1i(glGetUniformLocation(framebufferProgram, "DepthTexture"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, shadowMap.GetDepthMap());
        glUniform1i(glGetUniformLocation(framebufferProgram, "ShadowMap"), 2);

        glUniform1f(glGetUniformLocation(framebufferProgram, "near"), cam->getNear());
        glUniform1f(glGetUniformLocation(framebufferProgram, "far"), cam->getFar());

        Mat4 invProjection = Mat4::inverse(cam->getProjectionMatrix());
        Mat4 invView = Mat4::inverse(cam->getViewMatrix());
        glUniformMatrix4fv(glGetUniformLocation(framebufferProgram, "cameraInvProjection"), 1, GL_FALSE, invProjection.data());
        glUniformMatrix4fv(glGetUniformLocation(framebufferProgram, "cameraInvView"), 1, GL_FALSE, invView.data());
        glUniformMatrix4fv(glGetUniformLocation(framebufferProgram, "lightMatrix"), 1, GL_FALSE, shadowMap.GetLightSpaceMatrix().data());
        glUniform3f(glGetUniformLocation(framebufferProgram, "cameraPos"), cam->transform.getPosition().x, cam->transform.getPosition().y, cam->transform.getPosition().z);
        glUniform3f(glGetUniformLocation(framebufferProgram, "lightColor"), lightInfo.lightColor.x, lightInfo.lightColor.y, lightInfo.lightColor.z);
        glUniform1f(glGetUniformLocation(framebufferProgram, "time"), Time::time());
        glUniform1f(glGetUniformLocation(framebufferProgram, "lightShaftIntensity"), 0.2f);
        glDisable(GL_DEPTH_TEST);        
		glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);

        // Swap front and back buffers
        glfwSwapBuffers(windowManager->getWindow());

        // Poll for and process events
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