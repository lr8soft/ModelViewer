#include "ShaderManager.h"
#include <gl3w/gl3w.h>
#include "Utils/ShaderReader.h"

ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::bindProgram(ShaderData & data)
{

    GLuint programHandle = 0;
    //  Find shader
    if (shaderGroup.find(data.shaderName) != shaderGroup.end())
    {
        programHandle = shaderGroup[data.shaderName];
        glUseProgram(programHandle);
    }
    else {
        //  Init shader
        GLUtils::ShaderReader shader;
        shader.loadFromFile(data.vertexShader.c_str(), GL_VERTEX_SHADER);
        shader.loadFromFile(data.fragShader.c_str(), GL_FRAGMENT_SHADER);
        shader.linkAllShader();

        programHandle = shader.getProgramHandle();
        shaderGroup.insert(std::make_pair(data.shaderName, programHandle));
        glUseProgram(programHandle);
    }
    return programHandle;
}

bool ShaderManager::deleteProgram(ShaderData & data)
{
    auto targetIter = shaderGroup.find(data.shaderName);
    if (targetIter != shaderGroup.end())
    {
        glDeleteProgram(targetIter->second);
        shaderGroup.erase(targetIter);
        return true;
    }
    return false;
}


ShaderManager * ShaderManager::getInstance()
{
    return nullptr;
}
