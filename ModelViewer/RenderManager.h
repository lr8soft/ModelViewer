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
public:

    static RenderManager* getInstance();

    void OnInit();
    void OnRender();

    void initNewTrigger(std::shared_ptr<Event> event, EventTrigger trigger);
    void tryTriggerEvent(std::shared_ptr<Event> event);

};

#endif