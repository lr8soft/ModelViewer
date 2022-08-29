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

void EngineManager::initNewTrigger(std::shared_ptr<Event> event, EventTrigger trigger)
{
    eventBus.insert(std::make_pair(event->getEventName(), trigger));
}

void EngineManager::tryTriggerEvent(std::shared_ptr<Event> event)
{
    pendingTriggerList.push(event);
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
            auto event = pendingTriggerList.front();
            // find all triggers
            auto eventIter = eventBus.find(event->getEventName());
            while (eventIter != eventBus.end())
            {
                // check event is cancel
                if (event->isCancel())
                {
                    break;
                }
                // trigger work
                eventIter->second(*event);
                eventIter++;
            }
            // clean Event object
            pendingTriggerList.pop();
        }
    }
}
