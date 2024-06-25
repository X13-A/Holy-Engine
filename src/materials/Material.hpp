#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "../common/GLShader.h"
#include "GL/glew.h"
#include <iostream>

class Material
{
protected:

public:
    GLuint albedoTexID;
    std::string albedoPath;
    
    GLShader *shader;
    std::string vert_path;
    std::string frag_path;

    Material();
    ~Material();
    virtual void Init(std::string vert_path, std::string frag_path);
    virtual void SetUniforms();
    void LoadMap(const std::string& filename, GLuint* texID);
    void Release();
};

#endif
