#include "Grid.hpp"
#include "../common/GLShader.h"
#include "../math/Mat4.hpp"
#include <GLFW/glfw3.h>
#include "Transform.hpp"
#include "../camera/Camera.hpp"
#include "../math/Vec3.hpp"

void WireFrame::GenerateGrid(int size, float spacing)
{
    vertices.clear();
    indices.clear();

    for (int i = -size; i <= size; i++)
    {
        vertices.push_back(Vec3(-size * spacing, 0.0f, i * spacing));
        vertices.push_back(Vec3(size * spacing, 0.0f, i * spacing));

        vertices.push_back(Vec3(i * spacing, 0.0f, -size * spacing));
        vertices.push_back(Vec3(i * spacing, 0.0f, size * spacing));
    }

    for (int i = 0; i < vertices.size(); i += 2)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
    }
}

void WireFrame::Init()
{
    // Create and load shaders
    shader = new GLShader();
    if (!shader->LoadVertexShader("shaders/grid.vert.glsl"))
    {
        std::cerr << "Failed to load vertex shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->LoadFragmentShader("shaders/grid.frag.glsl"))
    {
        std::cerr << "Failed to load fragment shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->Create())
    {
        std::cerr << "Failed to create shader program" << std::endl;
        exit(EXIT_FAILURE);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLint loc_position = glGetAttribLocation(shader->GetProgram(), "a_position");

    glEnableVertexAttribArray(loc_position);

    glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*) 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void WireFrame::Destroy()
{
    shader->Destroy();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void WireFrame::Draw(Camera& camera)
{
    glUseProgram(shader->GetProgram());
    
    auto loc_viewMatrix = glGetUniformLocation(shader->GetProgram(), "viewMatrix");
    glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, camera.getViewMatrix().data());
    
    auto loc_projectionMatirx = glGetUniformLocation(shader->GetProgram(), "projectionMatrix");
    glUniformMatrix4fv(loc_projectionMatirx, 1, GL_FALSE, camera.getProjectionMatrix().data());

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind
    glBindVertexArray(0);
}