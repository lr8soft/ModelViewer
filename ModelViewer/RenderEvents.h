#pragma once
#ifndef _RENDER_EVENTS_H_
#define _RENDER_EVENTS_H_

#include "Events/Event.h"
namespace RenderEvents {
    //  EVENT_LOAD_NEW_MODEL, eventData is Model*
    void OnInitModel(Event& event);
}


#endif