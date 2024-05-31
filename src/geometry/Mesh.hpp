
#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "Vertex.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../common/stb_image.h"
#include "../../common/GLShader.h"

class Mesh
{
public:
    GLuint texID1, texID2;
    GLShader *shader;
    GLuint VBO, VAO, EBO;

    std::vector<Vertex> vertices = 
    {
               // Position                  // Color                // UV
        Vertex(Vec3(0.5f, 0.5f, 0.0f),   Vec3(1.0, 0.0, 0.0), Vec2(1, 1)), // top right
        Vertex(Vec3(0.5f, -0.5f, 0.0f),  Vec3(0.0, 1.0, 0.0), Vec2(1, 0)), // bottom right
        Vertex(Vec3(-0.5f, -0.5f, 0.0f), Vec3(0.0, 0.0, 1.0), Vec2(0, 0)), // bottom left
        Vertex(Vec3(-0.5f, 0.5f, 0.0f),  Vec3(1.0, 1.0, 1.0), Vec2(0, 1))  // top left
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 3,
        1, 2, 3
    };

    void Init()
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
        GLint loc_color = glGetAttribLocation(shader->GetProgram(), "a_color");
        GLint loc_uv = glGetAttribLocation(shader->GetProgram(), "a_uv");

        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_color);
        glEnableVertexAttribArray(loc_uv);

        glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
        glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));
        glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        SetTexture("textures/Grass.png", 0);
        SetTexture("textures/Stone.png", 1);
    }

    void Destroy()
    {
        shader->Destroy();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Draw()
    {
        glUseProgram(shader->GetProgram());
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID1);
        auto locationTexture0 = glGetUniformLocation(shader->GetProgram(), "u_sampler0");
        glUniform1i(locationTexture0, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texID2);
        auto locationTexture1 = glGetUniformLocation(shader->GetProgram(), "u_sampler1");
        glUniform1i(locationTexture1, 1);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, sizeof(indices) / sizeof(unsigned int));
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        // Unbind
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    void SetTexture(std::string filename, int slot = 0)
    {
        int width, height, comp;
        int desiredComp = 4;
        uint8_t *data = stbi_load(filename.c_str(), &width, &height, &comp, desiredComp);

        if (data)
        {
            GLuint* texID;
            if (slot == 0) texID = &texID1;
            if (slot == 1) texID = &texID2;

            glGenTextures(1, texID);
            glBindTexture(GL_TEXTURE_2D, *texID);

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

};

#endif