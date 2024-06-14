#include "Mesh.hpp"
#include "../common/stb_image.h"
#include "../common/GLShader.h"
#include "../math/Mat4.hpp"
#include <GLFW/glfw3.h>
#include "mesh_loader.hpp"
#include "Transform.hpp"
#include "../camera/Camera.hpp"

void Mesh::Load(const std::string& filename)
{
    MeshLoader loader;
    loader.load(filename, vertices, indices);
    std::cout << "- Vertices: " << vertices.size() << std::endl;
    std::cout << "- Indices: " << indices.size() << std::endl;
}

void Mesh::Init()
{
    // Create and load shaders
    shader = new GLShader();
    if (!shader->LoadVertexShader("shaders/vert.glsl"))
    {
        std::cerr << "Failed to load vertex shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->LoadFragmentShader("shaders/frag.glsl"))
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    GLint loc_position = glGetAttribLocation(shader->GetProgram(), "a_position");
    GLint loc_normal = glGetAttribLocation(shader->GetProgram(), "a_normal");
    GLint loc_uv = glGetAttribLocation(shader->GetProgram(), "a_uv");

    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_normal);
    glEnableVertexAttribArray(loc_uv);

    glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
    glVertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texcoord));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Destroy()
{
    shader->Destroy();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::Draw(Transform& transform, Camera& camera)
{
    glUseProgram(shader->GetProgram());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    auto loc_texture0 = glGetUniformLocation(shader->GetProgram(), "u_sampler0");
    glUniform1i(loc_texture0, 0);

    auto loc_modelMatrix = glGetUniformLocation(shader->GetProgram(), "modelMatrix");
    glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, transform.getTransformMatrix().data());

    auto loc_viewMatrix = glGetUniformLocation(shader->GetProgram(), "viewMatrix");
    glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, camera.getViewMatrix().data());

    auto loc_projectionMatirx = glGetUniformLocation(shader->GetProgram(), "projectionMatrix");
    glUniformMatrix4fv(loc_projectionMatirx, 1, GL_FALSE, camera.getProjectionMatrix().data());

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Mesh::SetTexture(const std::string& filename)
{
    int width, height, comp;
    int desiredComp = 4;
    uint8_t *data = stbi_load(filename.c_str(), &width, &height, &comp, desiredComp);

    if (data)
    {
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Generate mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free the image data
        stbi_image_free(data);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cout << "Loaded texture: " << filename << std::endl;
    }
    else
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
}