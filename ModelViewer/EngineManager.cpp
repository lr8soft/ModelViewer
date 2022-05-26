#include "EngineManager.h"

EngineManager* EngineManager::pInstance = nullptr;

EngineManager::EngineManager() {}

EngineManager * EngineManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new EngineManager;
    return pInstance;
}

void EngineManager::onLogicalWork()
{

}
