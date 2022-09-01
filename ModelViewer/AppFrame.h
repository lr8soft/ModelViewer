#pragma once
#ifndef _FRAME_WORK_
#define _FRAME_WORK_

#include <GLFW/glfw3.h>
#include <atomic>


class AppFrame
{
private:
    /*STATIC AREA*/
    static AppFrame *pInstance;
    static void FrameResize(GLFWwindow* screen, int w, int h);
    static void FramePos(GLFWwindow* screen, int x, int y);
    static void FrameCurseUpdate(GLFWwindow* screen, double x, double y);
    static void FrameScrollUpdate(GLFWwindow* screen, double x, double y);

    GLFWwindow *pScreen = nullptr;
    std::atomic_bool isFrameTerminate = false;
    bool isFullScreen = false;

    AppFrame();

public:
    static AppFrame* getInstance();
    bool getFrameTerminate();
    GLFWwindow* getScreen();

    void FrameInit();
    void FrameWork();
    void FrameFinalize();
};

#endif