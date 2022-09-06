#include "AppFrame.h"
#include "LogicalManager.h"
#include "RenderManager.h"

#include "Utils/LogUtil.h"

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

void LogicalManager::onMouseUpdate(double x, double y)
{
    mainCamera.processMouse(x, y);
}

void LogicalManager::onScrollUpdate(double x, double y)
{
    mainCamera.processScroll(y);
}

void LogicalManager::initNewTrigger(Event event, EventTrigger trigger)
{
    eventBus.insert(std::make_pair(event.getEventName(), trigger));
}

void LogicalManager::tryTriggerEvent(std::shared_ptr<Event> event)
{
    pendingTriggerList.push(event);
}

Camera * LogicalManager::getMainCamera()
{
    return &mainCamera;
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
        timer.Tick();
        float currentTime = timer.getAccumlateTime();
        // update main camera
        mainCamera.processInput((currentTime - lastUpdateTime));
        lastUpdateTime = timer.getAccumlateTime();
        // check pending size
        size_t pendingSize = pendingTriggerList.size();
        if (pendingSize == 0)
            continue;

        int lastIndex = pendingSize - 1;

        while (lastIndex >= 0)
        {
            std::shared_ptr<Event> currentEvent = pendingTriggerList.front();
            // find all triggers
            auto triggerIter = eventBus.lower_bound(currentEvent->getEventName());
            auto triggerEnd = eventBus.upper_bound(currentEvent->getEventName());
            while (triggerIter != triggerEnd)
            {
                // check event is cancel
                if (currentEvent->isCancel())
                {
                    break;
                }
                // trigger work
                triggerIter->second(*currentEvent);
                ++triggerIter;
            }
            // clean Event object
            pendingTriggerList.pop();
            // add event to last
            if (!currentEvent->isCancel() && currentEvent->isOngoingEvent())
            {
                pendingTriggerList.push(currentEvent);
            }
            lastIndex--;
        }
    }
}
