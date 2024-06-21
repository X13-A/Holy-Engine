#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "../common/GLShader.h"
#include "GL/glew.h"
#include <iostream>

class Material
{
protected:

public:
    GLuint texID;
    GLShader *shader;
    std::string vert_path;
    std::string frag_path;

    virtual void Init(std::string vert_path, std::string frag_path);
    virtual void SetUniforms();
    void SetTexture(const std::string &filename);
};

#endif