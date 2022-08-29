#include <gl3w/gl3w.h>

#include "RenderEvents.h"
#include "ModelManager.h"
#include "ShaderManager.h"

#include "Utils/LogUtil.hpp"

void RenderEvents::OnInitModel(Event& event)
{
    auto data = event.getEventData();
    if (data == nullptr)
    {
        LogUtil::printError("No model path!");
        return;
    }

    const char* modelPath = static_cast<const char*>(data);
    if (modelPath == nullptr)
    {
        LogUtil::printError("Not model path!");
        return;
    }

    ModelManager::getInstance()->LoadModel(modelPath);
    ModelManager::getInstance()->InitModel(modelPath);
}

void RenderEvents::OnLoadShader(Event & event)
{
    void* ptr = const_cast<void*>(event.getEventData());
    if (ptr == nullptr)
    {
        LogUtil::printError("No shader data!");
        return;
    }

    ShaderData* data = static_cast<ShaderData*>(ptr);
    if (data == nullptr)
    {
        LogUtil::printError("No shader data!");
        return;
    }

    ShaderManager::getInstance()->bindProgram(*data);
}

void RenderEvents::OnSendUniformData(Event & event)
{
    void* ptr = const_cast<void*>(event.getEventData());
    if (ptr == nullptr)
    {
        LogUtil::printError("No uniform data!");
        return;
    }

    UniformData* data = static_cast<UniformData*>(ptr);
    if (data == nullptr)
    {
        LogUtil::printError("No uniform data!");
        return;
    }

    switch (data->valueIndex)
    {
    case 0:
        glUniform1i(glGetUniformLocation(data->shaderId, data->attrName.c_str()), data->value.num0); break;
    case 1:
        glUniform1f(glGetUniformLocation(data->shaderId, data->attrName.c_str()), data->value.value1); break;
    case 2:
        glUniform2fv(glGetUniformLocation(data->shaderId, data->attrName.c_str()), 1, glm::value_ptr(data->value.vector2)); break;
    case 3:
        glUniform3fv(glGetUniformLocation(data->shaderId, data->attrName.c_str()), 1, glm::value_ptr(data->value.vector3)); break;
    case 4:
        glUniform4fv(glGetUniformLocation(data->shaderId, data->attrName.c_str()), 1, glm::value_ptr(data->value.vector4)); break;
    case 5:
        glUniformMatrix4fv(glGetUniformLocation(data->shaderId, data->attrName.c_str()), 1, false, glm::value_ptr(data->value.matrix5)); break;
    default:
        LogUtil::printError("Wrong uniform type!"); break;
    }

}


void RenderEvents::OnRenderModel(Event& event)
{
    void* data = const_cast<void*>(event.getEventData());
    if (data == nullptr)
    {
        LogUtil::printError("No render data!");
        return;
    }

    RenderData* renderData = static_cast<RenderData*>(data);
    if (renderData == nullptr)
    {
        LogUtil::printError("Not render data!");
        return;
    }

    ModelManager::getInstance()->RenderModel(renderData->modelName, renderData->shaderId, true, renderData->textureStartIndex);
}
