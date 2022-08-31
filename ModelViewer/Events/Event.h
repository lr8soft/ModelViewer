#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_
#include <string>
#include "EventDefine.h"

class Event {
protected:
    bool isEventCancel = false;
    bool isOngoing = false;
    std::string eventName;
    const void* const eventData;
public:
    Event(const std::string name, const void* const data = nullptr, bool ongoing = false): eventName(name),
        eventData(data) , isOngoing(ongoing) {}

    bool isCancel() { return isEventCancel; }
    bool isOngoingEvent() { return isOngoing; }
    void setIsCancel(bool value) { isEventCancel = value; }

    std::string getEventName() { return eventName; }
    const void* const getEventData() { return eventData; }
};

typedef void (*EventTrigger)(Event& event);

#endif