#pragma once
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_
#include <map>
#include <glcorearb.h>

#include "RenderData.h"
class ShaderManager {
private:
    static ShaderManager* pInstance;

    std::map<std::string, GLuint> shaderGroup;

    ShaderManager();
public:
    ~ShaderManager();


    GLuint bindProgram(ShaderData& data);

    bool deleteProgram(ShaderData& data);

    static ShaderManager * getInstance();

};

#endif