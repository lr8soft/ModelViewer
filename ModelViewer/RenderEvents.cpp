#include <gl3w/gl3w.h>

#include "Camera.h"
#include "RenderEvents.h"
#include "LogicalManager.h"
#include "LightManager.h"
#include "RenderManager.h"
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

    RenderManager::getInstance()->tryTriggerEvent(EVENT_SEND_LIGHT_DATA);
}

void RenderEvents::OnSendUniformData(Event & event)
{
    UniformData* data = getEventData<UniformData>(event);
    GLuint shaderId = ShaderManager::getInstance()->getCurrentShader()->shaderId;

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
    GLuint shaderId = ShaderManager::getInstance()->getCurrentShader()->shaderId;
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
    int shaderId = ShaderManager::getInstance()->getCurrentShader()->shaderId;
    LightManager::getInstance()->sendLightInfo(shaderId);
}

void RenderEvents::OnRenderModel(Event& event)
{
    if (event.isCancel()) return;

    RenderData* renderData = getEventData<RenderData>(event);
    GLuint shaderId = ShaderManager::getInstance()->getCurrentShader()->shaderId;

    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, renderData->transform.position);
    modelMatrix = glm::scale(modelMatrix, renderData->transform.scale);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(renderData->transform.rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(renderData->transform.rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(renderData->transform.rotation.z), glm::vec3(0, 0, 1));

    glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, false, glm::value_ptr(modelMatrix));

    if (renderData->renderShadow)
    {
        LightManager::getInstance()->sendLightMatrix(shaderId, renderData->transform);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shaderId, "material.shadow_map"), 0);
        glBindTexture(GL_TEXTURE_2D, PublicRenderData::depthMap);
    }

    ModelManager::getInstance()->RenderModel(
        renderData->modelName,
        shaderId,
        true,
        renderData->renderShadow ? 1 : 0);

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

void RenderEvents::OnRenderModelShadow(Event & event)
{
    if (event.isCancel()) return;

    RenderData* renderData = getEventData<RenderData>(event);
    Transform& transform = renderData->transform;

    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, transform.position);
    modelMatrix = glm::scale(modelMatrix, transform.scale);

    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));

    ShaderData* lastShader = ShaderManager::getInstance()->getCurrentShader();
    GLuint depthId = ShaderManager::getInstance()->bindProgram(PublicRenderData::shaderDepth);

    GLfloat near_panel = 0.1f, far_panel = 1000.0f;

    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_panel, far_panel);
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, PublicRenderData::depthMapFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto dirLights = LightManager::getInstance()->getDirectionalLights();
    for (auto dirLightData : dirLights) 
    {
        DirectionalLightData dirLight = dirLightData.second;
        glm::vec3 Front = transform.position + dirLight.direction;

        glm::mat4 lightView = glm::lookAt(transform.position - dirLight.direction, Front, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        glUniformMatrix4fv(glGetUniformLocation(depthId, "model"), 1, false, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(depthId, "lightSpaceMatrix"), 1, false, glm::value_ptr(lightSpaceMatrix));
        ModelManager::getInstance()->RenderModel(renderData->modelName, depthId, false, 0);
    }
    // recover environment
    ShaderManager::getInstance()->bindProgram(*lastShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
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
