#pragma once
#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_
#include <map>
#include <vector>
#include <string>
#include "LightData.h"

class LightManager {
private:
    std::map<std::string, PointLightData> pointLights;
    std::vector<const char*> pointLightNames;

    std::map<std::string, DirectionalLightData> dirLights;
    std::vector<const char*> dirLightNames;

    std::map<std::string, SpotLightData> spotLights;
    std::vector<const char*> spotLightNames;


    static LightManager* pInstance;

    LightManager();
public:

    static LightManager* getInstance();

    void addPointLight(std::string lightId, PointLightData data);
    void addDirectionalLight(std::string lightId, DirectionalLightData data);
    void addSpotLight(std::string lightId, SpotLightData data);
    void sendLightInfo(unsigned int shaderId);

    std::map<std::string, DirectionalLightData>& getDirectionalLights();

    const char** getAllPointLightName(int* count);
    const char** getAllDirectionalLightName(int* count);
    const char** getAllSpotLightName(int* count);

};

#endif