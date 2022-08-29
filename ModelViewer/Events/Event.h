#pragma once
#ifndef _EVENT_H_
#define _EVENT_H_
#include <string>

class Event {
public:
    virtual bool isCancellable() = 0;
    virtual std::string getEventName() = 0;
    virtual void onEventTrigger() = 0;
};

#endif