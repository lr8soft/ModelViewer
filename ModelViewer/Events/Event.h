#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_
#include <memory>
#include <string>
#include "EventDefine.h"

class Event {
protected:
    bool isEventCancel = false;
    bool isOngoing = false;
    bool isUniqueEvent = false;
    std::string eventName;
    const void* const eventData;
    std::shared_ptr<void> autoData = nullptr;
public:
    Event(const std::string name, const void* const data = nullptr, bool ongoing = false, bool unique = false): eventName(name),
        eventData(data) , isOngoing(ongoing) , isUniqueEvent(unique) {}

    explicit Event(const std::string name, std::shared_ptr<void> data, bool ongoing = false, bool unique = false): eventName(name),
        autoData(data), isOngoing(ongoing), isUniqueEvent(unique), eventData(nullptr) {}

    bool isCancel() { return isEventCancel; }
    bool isOngoingEvent() { return isOngoing; }
    bool isUnique() { return isUniqueEvent; }
    void setIsCancel(bool value) { isEventCancel = value; }
    void setIsGoging(bool value) { isOngoing = value; }

    std::string getEventName() { return eventName; }
    const void* const getEventData() 
    {
        if(autoData == nullptr)
            return eventData;
        return autoData.get();
    }
};

typedef void (*EventTrigger)(Event& event);

#endif