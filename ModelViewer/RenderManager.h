#pragma once
#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_
#include <map>

#include "Events/Event.h"
#include "Utils/SharedQueue.hpp"

class RenderManager 
{
private:
    static RenderManager* pInstance;

    // event name->triggers
    std::multimap<std::string, EventTrigger> eventBus;

    SharedQueue<std::shared_ptr<Event>> pendingTriggerList;

    RenderManager();

    void OnEventInit();
    void OnEventBusUpdate();

public:

    static RenderManager* getInstance();

    void OnInit();
    void OnRender();

    void initNewTrigger(Event event, EventTrigger trigger);
    void tryTriggerEvent(std::shared_ptr<Event> event);
    void tryTriggerEvent(const std::string& eventName, const void* const eventData = nullptr, bool isOngoing = false);
};

#endif