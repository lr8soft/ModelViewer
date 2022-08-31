#include <gl3w/gl3w.h>
#include <GLFW/glfw3.h>

#include "AppFrame.h"
#include "RenderManager.h"
#include "RenderEvents.h"
#include "UIManager.h"

#include "Utils/LogUtil.hpp"

RenderManager* RenderManager::pInstance = nullptr;

RenderManager::RenderManager() {}

RenderManager *RenderManager::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new RenderManager;
    return pInstance;
}

void RenderManager::OnInit()
{
    GLFWwindow* screen = AppFrame::getInstance()->getScreen();

    UIManager::OnRenderInit(screen);
    OnEventInit();
}

void RenderManager::OnEventInit()
{
    initNewTrigger(Event(EVENT_LOAD_NEW_MODEL), RenderEvents::OnInitModel);
    initNewTrigger(Event(EVENT_RENDER_MODEL), RenderEvents::OnRenderModel);
    initNewTrigger(Event(EVENT_LOAD_SHADER), RenderEvents::OnLoadShader);
    initNewTrigger(Event(EVENT_SEND_UNIFORM_DATA), RenderEvents::OnSendUniformData);

    static ShaderData data { "default", "Assets/default.vert", "Assets/default.frag" };
    tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_SHADER, &data));
}

void RenderManager::OnRender()
{
    GLFWwindow* screen = AppFrame::getInstance()->getScreen();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();

    UIManager::OnRenderUI();
    OnEventBusUpdate();

    glfwSwapBuffers(screen);
}

void RenderManager::OnEventBusUpdate()
{
    size_t pendingSize = pendingTriggerList.size();
    if (pendingSize == 0)
        return;

    int lastIndex = pendingSize - 1;

    while (lastIndex >= 0)
    {
        std::shared_ptr<Event> currentEvent = pendingTriggerList.front();
        // find all triggers
        auto triggerIter = eventBus.lower_bound(currentEvent->getEventName());
        auto triggerEnd = eventBus.upper_bound(currentEvent->getEventName());
        while (triggerIter != triggerEnd)
        {
            // check event is cancel
            if (currentEvent->isCancel())
            {
                break;
            }
            // trigger work
            triggerIter->second(*currentEvent);
            ++triggerIter;
        }
        // clean Event object
        pendingTriggerList.pop();
        // add event to last
        if (currentEvent->isOngoingEvent())
        {
            pendingTriggerList.push(currentEvent);
        }
        lastIndex--;
    }
}

void RenderManager::initNewTrigger(Event event, EventTrigger trigger)
{
    eventBus.insert(std::make_pair(event.getEventName(), trigger));
}

void RenderManager::tryTriggerEvent(std::shared_ptr<Event> event)
{
    pendingTriggerList.push(event);
}