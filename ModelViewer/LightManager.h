#pragma once
#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_
#include <map>
#include <string>
#include "LightData.h"

class LightManager {
private:
    std::map<std::string, PointLightData> pointLights;
    std::map<std::string, DirectionalLightData> dirLights;
    std::map<std::string, SpotLightData> spotLights;

    static LightManager* pInstance;

    LightManager();
public:

    static LightManager* getInstance();

    void sendLightInfo(unsigned int shaderId);
};

#endif