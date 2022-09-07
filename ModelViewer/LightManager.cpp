#include "LightManager.h"

LightManager* LightManager::pInstance = nullptr;

LightManager::LightManager() {}

LightManager * LightManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new LightManager;
    return pInstance;
}

void LightManager::sendLightInfo(unsigned int shaderId)
{
}
