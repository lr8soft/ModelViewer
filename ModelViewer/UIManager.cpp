#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

#include <Windows.h>
#include <commdlg.h>

#include "UIManager.h"
#include "LogicalManager.h"

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
    static char filePath[260] = {0};

    int bigBtnWidth = 100, bigBtnHeight = 30;
    if (isVisable)
    {
        ImGui::Begin("Open new model...", &isVisable);
        {
            if (ImGui::Button("Select..."))
            {
                OPENFILENAMEA openFileName;
                char szFile[260];
                ZeroMemory(&openFileName, sizeof(openFileName));
                openFileName.lStructSize = sizeof(openFileName);
                openFileName.hwndOwner = NULL;
                openFileName.lpstrFile = szFile;
                openFileName.lpstrFile[0] = '\0';
                openFileName.nMaxFile = sizeof(szFile);
                openFileName.lpstrFilter = "Model Files(*.obj)\0*.obj\0All Files(*.*)\0*.*\0\0";
                openFileName.nFilterIndex = 1;
                openFileName.lpstrFileTitle = NULL;
                openFileName.nMaxFileTitle = 0;
                openFileName.lpstrInitialDir = NULL;
                openFileName.Flags = 0;
                if (GetOpenFileNameA(&openFileName) == FALSE)
                {
                    LogUtil::printError("Can not open file!");
                }
                else
                {
                    memcpy_s(filePath, 260, openFileName.lpstrFile, 260);
                }
            }
            ImGui::SameLine();
            ImGui::InputText("Path", filePath, 255);

            if (ImGui::Button("Confirm", ImVec2(bigBtnWidth, bigBtnHeight)))
            {

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
