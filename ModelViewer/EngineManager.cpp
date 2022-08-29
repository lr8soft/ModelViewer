#include "AppFrame.h"
#include "EngineManager.h"

#include "Utils/LogUtil.hpp"

EngineManager* EngineManager::pInstance = nullptr;

EngineManager::EngineManager() {}

void EngineManager::onLogicalInit()
{
    if (logicalThread == nullptr)
    {
        // start logical thread
        logicalThread = new std::thread(&EngineManager::onLogicalWork, this);
        logicalThread->detach();
    }
}

void EngineManager::initNewEvent(std::string eventName, std::shared_ptr<Event> eventObject)
{
    eventBus.insert(std::make_pair(eventName, eventObject));
}

void EngineManager::tryTriggerEvent(std::string eventName)
{
}

EngineManager * EngineManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new EngineManager;
    return pInstance;
}

void EngineManager::onLogicalWork()
{
    LogUtil::printInfo("Logical thread start.");
    while (!AppFrame::getInstance()->getFrameTerminate())
    {
        LogUtil::printInfo("logical work");
    }
}
