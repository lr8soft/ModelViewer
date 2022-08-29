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
    std::multimap<std::string, std::shared_ptr<Event>> eventBus;

    SharedQueue<std::string> pendingTriggerList;

    EngineManager();

    void onLogicalWork();
public:

    static EngineManager* getInstance();

    void onLogicalInit();

    void initNewEvent(std::string eventName, std::shared_ptr<Event> eventObject);
    void tryTriggerEvent(std::string eventName);

};

#endif