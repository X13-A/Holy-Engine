#include "Material.hpp"
#include "../math/Vec3.hpp"
#include "../geometry/Transform.hpp"
#include "../camera/Camera.hpp"
#include "../light/SceneLightInfo.hpp"

#include "../common/stb_image.h"
#include "../common/GLShader.h"

void Material::Init(std::string vert_path, std::string frag_path)
{
    this->vert_path = vert_path;
    this->frag_path = frag_path;
    
    shader = new GLShader();
    if (!shader->LoadVertexShader(vert_path.c_str()))
    {
        std::cerr << "Failed to load vertex shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->LoadFragmentShader(frag_path.c_str()))
    {
        std::cerr << "Failed to load fragment shader" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!shader->Create())
    {
        std::cerr << "Failed to create shader program" << std::endl;
        exit(EXIT_FAILURE);
    }    
}

void Material::SetTexture(const std::string& filename)
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

void Material::SetUniforms()
{
    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    auto loc_texture = glGetUniformLocation(shader->GetProgram(), "albedoTexture");
    glUniform1i(loc_texture, 0);
}
