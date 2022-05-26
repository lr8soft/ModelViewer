#include <iostream>
#include <gl3w/gl3w.h>
#include "AppFrame.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

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

    ImGui::CreateContext();     // Setup Dear ImGui context
    ImGui::StyleColorsDark();       // Setup Dear ImGui style
    ImGui_ImplGlfw_InitForOpenGL(pScreen, true);     // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init("#version 450");
}

void AppFrame::FrameLoop()
{
    while (!glfwWindowShouldClose(pScreen)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(pScreen);

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
