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
    while (!pendingTriggerList.empty())
    {
        auto event = pendingTriggerList.front();
        // find all triggers
        auto eventIter = eventBus.lower_bound(event->getEventName());
        auto eventEnd = eventBus.upper_bound(event->getEventName());
        while (eventIter != eventEnd)
        {
            // check event is cancel
            if (event->isCancel())
            {
                break;
            }
            // trigger work
            eventIter->second(*event);
            ++eventIter;
        }
        // clean Event object
        pendingTriggerList.pop();
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
