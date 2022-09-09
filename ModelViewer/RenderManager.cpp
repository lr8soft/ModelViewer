#include <gl3w/gl3w.h>
#include <GLFW/glfw3.h>

#include "AppFrame.h"
#include "RenderManager.h"
#include "RenderEvents.h"
#include "PublicRenderData.h"
#include "UIManager.h"

#include "Utils/LogUtil.h"

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

    UIManager::OnInit(screen);
    OnEventInit();

    glGenFramebuffers(1, &PublicRenderData::depthMapFrameBuffer);
    glGenFramebuffers(1, &PublicRenderData::depthCubeMapFrameBuffer);
    glGenTextures(1, &PublicRenderData::depthMap);
    glBindTexture(GL_TEXTURE_2D, PublicRenderData::depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FrameInfo::ScreenWidth, FrameInfo::ScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, PublicRenderData::depthMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, PublicRenderData::depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &PublicRenderData::depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, PublicRenderData::depthCubeMap);
    for (GLuint i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
            FrameInfo::ScreenWidth, FrameInfo::ScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //attach depth cube texture
    glBindFramebuffer(GL_FRAMEBUFFER, PublicRenderData::depthCubeMapFrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, PublicRenderData::depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManager::OnEventInit()
{
    // Regist render event
    initNewTrigger(Event(EVENT_LOAD_NEW_MODEL), RenderEvents::OnInitModel);
    initNewTrigger(Event(EVENT_RENDER_MODEL), RenderEvents::OnRenderModel);
    initNewTrigger(Event(EVENT_RENDER_MODEL_SHADOW), RenderEvents::OnRenderModelShadow);
    initNewTrigger(Event(EVENT_STOP_RENDER_MODEL), RenderEvents::OnRenderCancel);
    initNewTrigger(Event(EVENT_LOAD_SHADER), RenderEvents::OnLoadShader);
    initNewTrigger(Event(EVENT_SEND_UNIFORM_DATA), RenderEvents::OnSendUniformData);
    initNewTrigger(Event(EVENT_SEND_UNIFORM_CAMERA_DATA), RenderEvents::OnSendCameraUniformData);
    initNewTrigger(Event(EVENT_SEND_LIGHT_DATA), RenderEvents::OnSendLightData);

    // Regist shaders
    tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_SHADER, &PublicRenderData::shaderDepth));
    tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_SHADER, &PublicRenderData::shaderNoTex));
    tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_SHADER, &PublicRenderData::shaderNoTexAndLight));
    tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_SHADER, &PublicRenderData::shaderDefaultNoShadow));
    tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_SHADER, &PublicRenderData::shaderDefault));

    // Trigger init event
    RenderManager::getInstance()->tryTriggerEvent(EVENT_SEND_LIGHT_DATA);

    glEnable(GL_DEPTH_TEST);
}

void RenderManager::OnRender()
{
    GLFWwindow* screen = AppFrame::getInstance()->getScreen();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();

    OnEventBusUpdate();
    UIManager::OnRenderUI();

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
        if (!currentEvent->isCancel() && currentEvent->isOngoingEvent())
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

void RenderManager::tryTriggerEvent(const std::string & eventName, const void * const eventData, bool ongoing)
{
    pendingTriggerList.push(std::make_shared<Event>(eventName, eventData, ongoing));
}

void RenderManager::tryTriggerEvent(const std::string & eventName, std::shared_ptr<void> eventData, bool isOngoing)
{
    pendingTriggerList.push(std::make_shared<Event>(eventName, eventData, isOngoing));
}
