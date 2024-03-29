#pragma once
#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

struct GLFWwindow;
class UIManager {
private:
    static void RenderLoaderPanel();
    static void RenderShaderSelectorPanel();
    static void RenderPointLightPanel();
    static void RenderDirLightPanel();
    static void RenderSpotLightPanel();

public:
    static void OnInit(GLFWwindow* pScreen);
    static void OnRenderUI();
    static void OnFinalize();

};

#endif