#include "ShaderManager.h"
#include <gl3w/gl3w.h>
#include "Utils/ShaderReader.h"

ShaderManager* ShaderManager::pInstance = nullptr;
ShaderManager::ShaderManager() {}
ShaderManager::~ShaderManager() {}

GLuint ShaderManager::bindProgram(std::string shaderName)
{
    GLuint programHandle = getShaderId(shaderName);
    if (programHandle != 0)
    {
        currentShader = &shaderGroup[shaderName];
        programHandle = currentShader->shaderId;

        glUseProgram(programHandle);
    }
    return programHandle;
}

GLuint ShaderManager::bindProgram(ShaderData & data)
{
    GLuint programHandle = bindProgram(data.shaderName);
    //  Find shader
    if (programHandle == 0)
    {
        //  Init shader
        GLUtils::ShaderReader shader;
        shader.loadFromFile(data.vertexShader.c_str(), GL_VERTEX_SHADER);
        shader.loadFromFile(data.fragShader.c_str(), GL_FRAGMENT_SHADER);
        shader.linkAllShader();

        programHandle = shader.getProgramHandle();
        data.shaderId = programHandle;

        shaderGroup.insert(std::make_pair(data.shaderName, data));
        recordShaders.push_back(data.shaderName.c_str());

        currentShader = &data;
        glUseProgram(programHandle);
    }
    return programHandle;
}

bool ShaderManager::deleteProgram(ShaderData & data)
{
    auto targetIter = shaderGroup.find(data.shaderName);
    if (targetIter != shaderGroup.end())
    {
        glDeleteProgram(targetIter->second.shaderId);
        shaderGroup.erase(targetIter);
        return true;
    }
    return false;
}

const char ** ShaderManager::getAllShadersName(int * shaderCount)
{
    *shaderCount = recordShaders.size();
    return &recordShaders[0];
}

ShaderData * ShaderManager::getCurrentShader()
{
    return currentShader;
}

ShaderData* ShaderManager::getShader(std::string shaderName)
{
    if (shaderGroup.find(shaderName) != shaderGroup.end())
    {
        return &shaderGroup[shaderName];
    }
    return nullptr;
}

GLuint ShaderManager::getShaderId(std::string shaderName)
{
    ShaderData* data = getShader(shaderName);
    if (data != nullptr)
        return data->shaderId;

    return 0;
}


ShaderManager * ShaderManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new ShaderManager;
    return pInstance;
}
