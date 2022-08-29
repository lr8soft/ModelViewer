#include "AppFrame.h"
#include "LogicalManager.h"
#include "RenderManager.h"

#include "Utils/LogUtil.hpp"

LogicalManager* LogicalManager::pInstance = nullptr;

LogicalManager::LogicalManager() {}

void LogicalManager::onLogicalInit()
{
    if (logicalThread == nullptr)
    {
        // start logical thread
        logicalThread = new std::thread(&LogicalManager::onLogicalWork, this);
        logicalThread->detach();
    }
}

void LogicalManager::initNewTrigger(Event event, EventTrigger trigger)
{
    eventBus.insert(std::make_pair(event.getEventName(), trigger));
}

void LogicalManager::tryTriggerEvent(std::shared_ptr<Event> event)
{
    pendingTriggerList.push(event);
}

LogicalManager * LogicalManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new LogicalManager;
    return pInstance;
}

void LogicalManager::onLogicalWork()
{
#if _DEBUG
    LogUtil::printWarn("Logical thread start.");
#endif
    while (!AppFrame::getInstance()->getFrameTerminate())
    {
        //RenderManager::getInstance()->tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_NEW_MODEL, "H:\\ModelViewer\\ModelViewer\\Assets\\1.FBX"));

        while (!pendingTriggerList.empty())
        {
            auto event = pendingTriggerList.front();
            // find all triggers
            auto eventIter = eventBus.lower_bound(event->getEventName());
            auto eventEnd = eventBus.upper_bound(event->getEventName());
            while (eventIter != eventEnd)
            {
                // check event is cancel
                if (event->isCancel())
                {
                    break;
                }
                // trigger work
                eventIter->second(*event);
                ++eventIter;
            }
            // clean Event object
            pendingTriggerList.pop();
        }
    }
}
