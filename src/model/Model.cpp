#include "Model.hpp"
#include "../materials/Material.hpp"
#include "../geometry/Mesh.hpp"
#include "../geometry/Transform.hpp"
#include "../geometry/Vertex.hpp"

void Model::Init()
{
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    // GLint shader = material->shader->GetProgram();
    GLint error = glGetError();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    error = glGetError();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    error = glGetError();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
    error = glGetError();
    glEnableVertexAttribArray(0);
    error = glGetError();
    glEnableVertexAttribArray(1);
    error = glGetError();
    glEnableVertexAttribArray(2);
    error = glGetError();

    if (error != GL_NO_ERROR) 
    {
        std::cerr << "OpenGL error before drawing: " << error << std::endl;
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::Draw(Camera* camera)
{
    uint32_t shader = material->shader->GetProgram();
    
    glUseProgram(shader);

    // Base uniforms, they don't depend on the material
    auto loc_modelMatrix = glGetUniformLocation(shader, "modelMatrix");
    glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, transform->getTransformMatrix().data());

    auto loc_viewMatrix = glGetUniformLocation(shader, "viewMatrix");
    glUniformMatrix4fv(loc_viewMatrix, 1, GL_FALSE, camera->getViewMatrix().data());

    auto loc_projectionMatrix = glGetUniformLocation(shader, "projectionMatrix");
    glUniformMatrix4fv(loc_projectionMatrix, 1, GL_FALSE, camera->getProjectionMatrix().data());
    
    material->SetUniforms();

    // Draw the mesh
    glBindVertexArray(mesh->VAO);

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}