#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_
#include <string>
#include "EventDefine.h"

class Event {
protected:
    bool isEventCancel = false;
    std::string eventName;
    const void* const eventData;
public:
    Event(const std::string name, const void* const data = nullptr): eventName(name), eventData(data) {}

    bool isCancel() { return isEventCancel; }
    void setIsCancel(bool value) { isEventCancel = value; }

    std::string getEventName() { return eventName; }
    const void* const getEventData() { return eventData; }
};

typedef void (*EventTrigger)(Event& event);

#endif