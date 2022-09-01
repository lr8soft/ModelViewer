#pragma once
#ifndef _ENGINE_MANAGER_H_
#define _ENGINE_MANAGER_H_

#include <thread>
#include <string>
#include <map>
#include <memory>

#include "Camera.h"

#include "Events/Event.h"
#include "Utils/Timer.h"
#include "Utils/SharedQueue.hpp"

class LogicalManager 
{
private:
    static LogicalManager* pInstance;

    std::thread* logicalThread = nullptr;
    // event name->triggers
    std::multimap<std::string, EventTrigger> eventBus;

    SharedQueue<std::shared_ptr<Event>> pendingTriggerList;

    Camera mainCamera;

    Timer timer;

    float lastUpdateTime = 0.0f;

    LogicalManager();

    void onLogicalWork();
public:
    static LogicalManager* getInstance();

    void onLogicalInit();

    void onMouseUpdate(double x, double y);
    void onScrollUpdate(double x, double y);

    void initNewTrigger(Event event, EventTrigger trigger);
    void tryTriggerEvent(std::shared_ptr<Event> event);

    Camera* getMainCamera();

};

#endif