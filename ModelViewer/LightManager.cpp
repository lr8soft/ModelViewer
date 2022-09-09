#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "LightManager.h"
#include "RenderManager.h"

LightManager* LightManager::pInstance = nullptr;

LightManager::LightManager() {}

LightManager * LightManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new LightManager;
    return pInstance;
}

void LightManager::addPointLight(std::string lightId, PointLightData data)
{
    size_t strLen = lightId.size() + 1;
    if (strLen == 0) return;

    pointLights.insert(std::make_pair(lightId, data));

    char* info = new char[strLen];
    memcpy(info, lightId.c_str(), strLen);
    memset(info + strLen, 0, 1);

    pointLightNames.push_back(info);
}

void LightManager::addDirectionalLight(std::string lightId, DirectionalLightData data)
{
    size_t strLen = lightId.size() + 1;
    if (strLen == 0) return;

    dirLights.insert(std::make_pair(lightId, data));

    char* info = new char[strLen];
    memcpy(info, lightId.c_str(), strLen);
    memset(info + strLen, 0, 1);

    dirLightNames.push_back(info);
}

void LightManager::addSpotLight(std::string lightId, SpotLightData data)
{
    size_t strLen = lightId.size() + 1;
    if (strLen == 0) return;
    spotLights.insert(std::make_pair(lightId, data));

    char* info = new char[strLen];
    memcpy(info, lightId.c_str(), strLen);
    memset(info + strLen, 0, 1);

    spotLightNames.push_back(info);
}

void LightManager::sendLightInfo(unsigned int shaderId)
{
    glm::vec3 lightInfo;
    lightInfo.x = dirLights.size();
    lightInfo.y = pointLights.size();
    lightInfo.z = spotLights.size();
    // send light sources count
    glUniform3fv(glGetUniformLocation(shaderId, "lightCount"), 1, glm::value_ptr(lightInfo));

    char segmentName[32];
    // send all point light data
    int pointLightIndex = 0;
    for (auto lightPair : pointLights)
    {
        PointLightData lightData = lightPair.second;

        sprintf_s(segmentName, 32, "pointLights[%d].position", pointLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(lightData.position));

        std::string info = segmentName;

        sprintf_s(segmentName, 32, "pointLights[%d].ambient", pointLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(lightData.ambient));

        sprintf_s(segmentName, 32, "pointLights[%d].diffuse", pointLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(lightData.diffuse));

        sprintf_s(segmentName, 32, "pointLights[%d].specular", pointLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(lightData.specular));

        sprintf_s(segmentName, 32, "pointLights[%d].constant", pointLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), lightData.constant);

        sprintf_s(segmentName, 32, "pointLights[%d].linear", pointLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), lightData.linear);

        sprintf_s(segmentName, 32, "pointLights[%d].quadratic", pointLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), lightData.quadratic);

        pointLightIndex++;
    }
    // send all directional light data
    int dirLightIndex = 0;
    for (auto dirLightData : dirLights)
    {
        DirectionalLightData light = dirLightData.second;

        sprintf_s(segmentName, 32, "dirLight[%d].direction", dirLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(light.direction));

        sprintf_s(segmentName, 32, "dirLight[%d].ambient", dirLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(light.ambient));

        sprintf_s(segmentName, 32, "dirLight[%d].diffuse", dirLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(light.diffuse));

        sprintf_s(segmentName, 32, "dirLight[%d].specular", dirLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(light.specular));

        dirLightIndex++;
    }
    // send all spot light data
    int spotLightIndex = 0;
    for (auto flashLightData : spotLights)
    {
        SpotLightData spotLight = flashLightData.second;

        sprintf_s(segmentName, 32, "spotLight[%d].position", spotLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(spotLight.position));

        sprintf_s(segmentName, 32, "spotLight[%d].direction", spotLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(spotLight.direction));

        sprintf_s(segmentName, 32, "spotLight[%d].cutOff", spotLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), spotLight.cutOff);

        sprintf_s(segmentName, 32, "spotLight[%d].outerCutOff", spotLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), spotLight.outerCutOff);

        sprintf_s(segmentName, 32, "spotLight[%d].constant", spotLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), spotLight.constant);

        sprintf_s(segmentName, 32, "spotLight[%d].linear", spotLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), spotLight.linear);

        sprintf_s(segmentName, 32, "spotLight[%d].quadratic", spotLightIndex);
        glUniform1f(glGetUniformLocation(shaderId, segmentName), spotLight.quadratic);

        sprintf_s(segmentName, 32, "spotLight[%d].ambient", spotLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(spotLight.ambient));

        sprintf_s(segmentName, 32, "spotLight[%d].diffuse", spotLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(spotLight.diffuse));

        sprintf_s(segmentName, 32, "spotLight[%d].specular", spotLightIndex);
        glUniform3fv(glGetUniformLocation(shaderId, segmentName), 1, glm::value_ptr(spotLight.specular));

        spotLightIndex++;
    }
}

void LightManager::sendLightMatrix(unsigned int shaderId, Transform & transform)
{
    int dirLightIndex = 0;
    char segmentName[32];

    GLfloat near_panel = 1.0f, far_panel = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_panel, far_panel);
    

    for (auto dirLightData : dirLights)
    {
        DirectionalLightData light = dirLightData.second;

        glm::vec3 Front = transform.position + light.direction * 2.5f;

        glm::mat4 lightView = glm::lookAt(transform.position - light.direction * 2.5f, Front, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        sprintf_s(segmentName, 32, "dirShadowLightSpace[%d]", dirLightIndex++);
        glUniformMatrix4fv(glGetUniformLocation(shaderId, segmentName), 1, false, glm::value_ptr(lightSpaceMatrix));
    }
}

std::multimap<std::string, DirectionalLightData>& LightManager::getDirectionalLights()
{
    return dirLights;
}

const char ** LightManager::getAllPointLightName(int * count)
{
    int lightCount = pointLightNames.size();
    (*count) = lightCount;
    if (lightCount == 0)
    {
        static const char* nullStr = "";
        return &nullStr;
    }
    return &pointLightNames[0];
}

const char ** LightManager::getAllDirectionalLightName(int * count)
{
    int lightCount = dirLightNames.size();
    (*count) = lightCount;
    if (lightCount == 0)
    {
        static const char* nullStr = "";
        return &nullStr;
    }
    return &dirLightNames[0];
}

const char ** LightManager::getAllSpotLightName(int * count)
{
    int lightCount = spotLightNames.size();
    (*count) = lightCount;
    if (lightCount == 0)
    {
        static const char* nullStr = "";
        return &nullStr;
    }
    return &spotLightNames[0];
}
