#include <iostream>
#include <gl3w/gl3w.h>

#include "AppFrame.h"

AppFrame* AppFrame::pInstance = nullptr;

AppFrame::AppFrame()
{
}

void AppFrame::FrameResize(GLFWwindow * screen, int w, int h)
{
}
void AppFrame::FramePos(GLFWwindow * screen, int x, int y)
{
}
void AppFrame::FrameCurseUpdate(GLFWwindow * screen, double x, double y)
{
}
void AppFrame::FrameScrollUpdate(GLFWwindow * screen, double x, double y)
{
}

AppFrame * AppFrame::getInstance()
{
    if (pInstance == nullptr)
        pInstance = new AppFrame;

    return pInstance;
}

bool AppFrame::getFrameTerminate()
{
    return false;
}

GLFWwindow * AppFrame::getScreen()
{
    return nullptr;
}

void AppFrame::FrameInit()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, false);//Auto change size

    pScreen = glfwCreateWindow(1280, 720, "ModelViewer", nullptr, nullptr);
    glfwMakeContextCurrent(pScreen);

    if (pScreen == nullptr)
    {
        std::cerr << "[ERROR] Failed to create window!\n";
        exit(-1);
    }


    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(pScreen, FrameResize);
    glfwSetWindowPosCallback(pScreen, FramePos);

    glfwSetCursorPosCallback(pScreen, FrameCurseUpdate);
    glfwSetScrollCallback(pScreen, FrameScrollUpdate);

    gl3wInit();
}

void AppFrame::FrameLoop()
{
    while (!glfwWindowShouldClose(pScreen)) {

        glfwSwapBuffers(pScreen);
        glfwPollEvents();
    }
    isFrameTerminate = true;

    glfwDestroyWindow(pScreen);
    glfwDestroyWindow(tScreen);
    glfwTerminate();
}

void AppFrame::FrameFinalize()
{
    delete pInstance;
    pInstance = nullptr;
}