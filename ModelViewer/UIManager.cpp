#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>


#include "UIManager.h"
#include "EngineManager.h"

#include "Utils/LogUtil.hpp"

void testTrigger(Event& event)
{
    LogUtil::printError(event.getEventName() + " is click.");
}

void UIManager::OnRenderInit(GLFWwindow* pScreen)
{
    ImGui::CreateContext();     // Setup Dear ImGui context
    ImGui::StyleColorsDark();       // Setup Dear ImGui style
    ImGui_ImplGlfw_InitForOpenGL(pScreen, true);     // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init("#version 450");
}



void UIManager::RenderLoaderPanel()
{
    static bool isVisable = true;
    static char filePath[256] = {0};

    int bigBtnWidth = 100, bigBtnHeight = 30;
    if (isVisable)
    {
        ImGui::Begin("Open new model...", &isVisable);
        {
            if (ImGui::Button("Select..."))
            {

            }
            ImGui::SameLine();
            ImGui::InputText("Path", filePath, 255);
            ImGui::NewLine();
            if (ImGui::Button("Confirm", ImVec2(bigBtnWidth, bigBtnHeight)))
            {
                LogUtil::printInfo(filePath);
            }

            ImGui::SameLine();
            if (ImGui::Button("Clear", ImVec2(bigBtnWidth, bigBtnHeight)))
            {

            }
        }
        ImGui::End();
    }

}

void UIManager::OnRenderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderLoaderPanel();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
