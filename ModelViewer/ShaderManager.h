#pragma once
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_
#include <map>
#include <vector>
#include <string>
#include <glcorearb.h>

#include "RenderData.h"
class ShaderManager {
private:
    static ShaderManager* pInstance;

    std::map<std::string, GLuint> shaderGroup;
    std::vector<const char*> recordShaders;

    ShaderManager();
public:
    ~ShaderManager();
    // bind shader
    GLuint bindProgram(std::string shaderName);
    // init and bind shader
    GLuint bindProgram(ShaderData& data);

    bool deleteProgram(ShaderData& data);

    const char** getAllShadersName(int* shaderCount);

    static ShaderManager * getInstance();

};

#endif