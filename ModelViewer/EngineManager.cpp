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
    pendingTriggerList.push(eventName);
}

EngineManager * EngineManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new EngineManager;
    return pInstance;
}

void EngineManager::onLogicalWork()
{
    while (!AppFrame::getInstance()->getFrameTerminate())
    {
        while (!pendingTriggerList.empty())
        {
            std::string eventName = pendingTriggerList.front();

            LogUtil::printInfo("Try trigger event" + eventName);
            auto eventIter = eventBus.find(eventName);
            while (eventIter != eventBus.end())
            {
                eventIter->second->onEventTrigger();
                eventIter++;
            }

            pendingTriggerList.pop();
        }
    }
}
