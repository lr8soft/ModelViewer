#pragma once
#ifndef _RENDER_EVENTS_H_
#define _RENDER_EVENTS_H_
#include "RenderData.h"
#include "Events/Event.h"
namespace RenderEvents {
    //  EVENT_LOAD_NEW_MODEL, eventData == model path (const char*)
    void OnInitModel(Event& event);
    //  EVENT_LOAD_SHADER, eventData == shader path (const char*)
    void OnLoadShader(Event& event);
    // EVENT_SEND_UNIFORM_DATA, eventData == UniformData*
    void OnSendUniformData(Event& event);
    //  EVENT_RENDER_MODEL, eventData == RenderData
    void OnRenderModel(Event& event);
}


#endif