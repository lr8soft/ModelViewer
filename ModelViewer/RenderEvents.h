#pragma once
#ifndef _RENDER_EVENTS_H_
#define _RENDER_EVENTS_H_
#include <string>
#include <map>
#include "RenderData.h"
#include "Events/Event.h"

#include "Utils/LogUtil.h"
namespace RenderEvents {
    class PublicRenderData {
    public:
        static std::multimap<std::string, Event&> renderingModels;
    };

    template<class T> T* getEventData(Event& event)
    {
        auto ptr = event.getEventData();
        if (ptr == nullptr)
        {
            LogUtil::printError("No event data!");
            return nullptr;
        }

        return (T*)(ptr);
    }

    //  EVENT_LOAD_NEW_MODEL, eventData == model path (const char*)
    void OnInitModel(Event& event);
    //  EVENT_LOAD_SHADER, eventData == shader path (const char*)
    void OnLoadShader(Event& event);
    //  EVENT_RENDER_MODEL, eventData == RenderData
    void OnRenderModel(Event& event);

    // EVENT_SEND_UNIFORM_DATA, eventData == UniformData*
    void OnSendUniformData(Event& event);
    // EVENT_SEND_UNIFORM_CAMERA_DATA, eventData == shader path (const char*)
    // Send matrix "view" "projection" to target shader
    void OnSendCameraUniformData(Event& event);

}


#endif