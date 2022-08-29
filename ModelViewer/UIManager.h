#pragma once
#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

struct GLFWwindow;
class UIManager {
private:
    static void RenderLoaderPanel();
public:
    static void OnRenderInit(GLFWwindow* pScreen);
    static void OnRenderUI();

};

#endif