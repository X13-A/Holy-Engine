#include "Shape.hpp"
#include "../geometry/Transform.hpp"

void Shape::Init()
{
    mesh->Init();
    material->Init("shaders/vert.glsl", "shaders/frag.glsl"); // TODO: move consts

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shape::Draw(Camera* camera, Transform* transform)
{
    uint32_t shader = material->shader->GetProgram();
    
    glUseProgram(shader);

    // Base uniforms, they don't depend on the material
    auto loc_modelMatrix = glGetUniformLocation(shader, "modelMatrix");
    glUniformMatrix4fv(loc_modelMatrix, 1, GL_FALSE, transform->getTransformMatrix().data());

    auto loc_normalMatrix = glGetUniformLocation(shader, "normalMatrix");
    Mat4 normalMatrix = Mat4::transpose(Mat4::inverse(transform->getTransformMatrix()));
    glUniformMatrix4fv(loc_normalMatrix, 1, GL_FALSE, normalMatrix.data());

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
