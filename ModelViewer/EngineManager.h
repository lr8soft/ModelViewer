#pragma once
#ifndef _ENGINE_MANAGER_H_
#define _ENGINE_MANAGER_H_

#include <thread>
#include <string>
#include <map>
#include <memory>

#include "Events/Event.h"
#include "Utils/Timer.h"
#include "Utils/SharedQueue.hpp"

class EngineManager 
{
private:
    static EngineManager* pInstance;

    std::thread* logicalThread = nullptr;
    // event name->triggers
    std::multimap<std::string, EventTrigger> eventBus;

    SharedQueue<std::shared_ptr<Event>> pendingTriggerList;

    EngineManager();

    void onLogicalWork();
public:
    static EngineManager* getInstance();

    void onLogicalInit();

    void initNewTrigger(std::shared_ptr<Event> event, EventTrigger trigger);
    void tryTriggerEvent(std::shared_ptr<Event> event);

};

#endif