#include <gl3w/gl3w.h>

#include "Camera.h"
#include "RenderEvents.h"
#include "LogicalManager.h"
#include "LightManager.h"
#include "PublicRenderData.h"
#include "ModelManager.h"
#include "ShaderManager.h"

void RenderEvents::OnInitModel(Event& event)
{
    const char* modelPath = getEventData<const char>(event);

    ModelManager::getInstance()->LoadModel(modelPath);
    ModelManager::getInstance()->InitModel(modelPath);
}

void RenderEvents::OnLoadShader(Event & event)
{
    ShaderData* data = getEventData<ShaderData>(event);
    ShaderManager::getInstance()->bindProgram(*data);
}

void RenderEvents::OnSendUniformData(Event & event)
{
    UniformData* data = getEventData<UniformData>(event);
    GLuint shaderId = ShaderManager::getInstance()->getCurrentShaderId();

    switch (data->valueIndex)
    {
    case 0:
        glUniform1i(glGetUniformLocation(shaderId, data->attrName.c_str()), data->value.num0); break;
    case 1:
        glUniform1f(glGetUniformLocation(shaderId, data->attrName.c_str()), data->value.value1); break;
    case 2:
        glUniform2fv(glGetUniformLocation(shaderId, data->attrName.c_str()), 1, glm::value_ptr(data->value.vector2)); break;
    case 3:
        glUniform3fv(glGetUniformLocation(shaderId, data->attrName.c_str()), 1, glm::value_ptr(data->value.vector3)); break;
    case 4:
        glUniform4fv(glGetUniformLocation(shaderId, data->attrName.c_str()), 1, glm::value_ptr(data->value.vector4)); break;
    case 5:
        glUniformMatrix4fv(glGetUniformLocation(shaderId, data->attrName.c_str()), 1, false, glm::value_ptr(data->value.matrix5)); break;
    default:
        LogUtil::printError("Wrong uniform type!"); break;
    }
}

void RenderEvents::OnSendCameraUniformData(Event & event)
{
    GLuint shaderId = ShaderManager::getInstance()->getCurrentShaderId();
    Camera* camera = LogicalManager::getInstance()->getMainCamera();

    glm::mat4 viewMat = camera->getViewMatrix();
    glm::mat4 projectionMat = camera->getProjectionMatrix();

    glm::vec3 viewPos = camera->getPostion();

    glUniform3fv(glGetUniformLocation(shaderId, "viewPos"), 1, glm::value_ptr(viewPos));

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, false, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, false, glm::value_ptr(projectionMat));
}

void RenderEvents::OnSendLightData(Event & event)
{
    int shaderId = ShaderManager::getInstance()->getCurrentShaderId();
    LightManager::getInstance()->sendLightInfo(shaderId);
}

void RenderEvents::OnRenderModel(Event& event)
{
    if (event.isCancel()) return;

    RenderData* renderData = getEventData<RenderData>(event);

    ModelManager::getInstance()->RenderModel(
        renderData->modelName,
        ShaderManager::getInstance()->getCurrentShaderId(),
        true,
        renderData->textureStartIndex);

    auto beg = PublicRenderData::renderingModelEvents.lower_bound(renderData->modelName);
    auto end = PublicRenderData::renderingModelEvents.upper_bound(renderData->modelName);
    // check had storaged the render event
    bool isAppear = false;
    while (beg != end)
    {
        if (&(beg->second) == &event ||
            ((event.getEventData()!= nullptr) && event.getEventData() == beg->second.getEventData()) )
        {
            isAppear = true;
            break;
        }
        ++beg;
    }

    if (!isAppear)
    {
        std::lock_guard<std::mutex> gurad(PublicRenderData::modelMutex);
        PublicRenderData::renderingModelEvents.insert(std::pair<std::string, Event&>(renderData->modelName, event));

        PublicRenderData::renderingModelNames.push_back(renderData->modelName.c_str());
    }

}

void RenderEvents::OnRenderCancel(Event& event)
{
    const char* modelName = getEventData<const char>(event);

    auto modelIter = PublicRenderData::renderingModelEvents.find(modelName);
    if (modelIter != PublicRenderData::renderingModelEvents.end())
    {
        modelIter->second.setIsCancel(true);
        modelIter->second.setIsGoging(false);
        PublicRenderData::renderingModelEvents.erase(modelIter);

        auto targetIter = std::find(
            PublicRenderData::renderingModelNames.begin(), PublicRenderData::renderingModelNames.end(), modelName);
        if (targetIter != PublicRenderData::renderingModelNames.end())
        {
            PublicRenderData::renderingModelNames.erase(targetIter);
        }
    }
    else
    {
        LogUtil::printError("Unexist model!");
    }
}
