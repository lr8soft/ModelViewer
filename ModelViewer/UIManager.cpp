#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

#include <Windows.h>
#include <commdlg.h>

#include "UIManager.h"
#include "RenderData.h"
#include "RenderManager.h"
#include "LogicalManager.h"
#include "ShaderManager.h"

#include "Utils/LogUtil.h"

void testTrigger(Event& event)
{
    LogUtil::printError(event.getEventName() + " is click.");
}

void UIManager::OnInit(GLFWwindow* pScreen)
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
                openFileName.lpstrFilter = "All Files(*.*)\0*.*\0\0";
                openFileName.nFilterIndex = 1;
                openFileName.lpstrFileTitle = NULL;
                openFileName.nMaxFileTitle = 0;
                openFileName.lpstrInitialDir = NULL;
                openFileName.Flags = 0;
                if (GetOpenFileNameA(&openFileName) == TRUE)
                {
                    memcpy_s(filePath, 260, openFileName.lpstrFile, 260);
                }
            }
            ImGui::SameLine();
            ImGui::InputText("Path", filePath, 255);
            // load model and show
            if (ImGui::Button("Confirm", ImVec2(bigBtnWidth, bigBtnHeight)))
            {
                RenderManager::getInstance()->tryTriggerEvent(std::make_shared<Event>(EVENT_LOAD_NEW_MODEL, filePath));

                Camera* camera = LogicalManager::getInstance()->getMainCamera();

                glm::mat4 matrix;
                matrix = glm::translate(matrix, glm::vec3(0.0));


                // send mvp matrix
                static UniformData mvpMatrix;
                mvpMatrix.attrName = "model";
                mvpMatrix.value.matrix5 = matrix ;
                mvpMatrix.valueIndex = 5;

                RenderManager::getInstance()->tryTriggerEvent(std::make_shared<Event>(EVENT_SEND_UNIFORM_DATA, &mvpMatrix));
                RenderManager::getInstance()->tryTriggerEvent(EVENT_SEND_UNIFORM_CAMERA_DATA, "default", true);

                static RenderData data;
                data.shaderName = "default";
                data.modelName = filePath;
                RenderManager::getInstance()->tryTriggerEvent(std::make_shared<Event>(EVENT_RENDER_MODEL, &data, true));
            }

            ImGui::SameLine();
            if (ImGui::Button("Clear", ImVec2(bigBtnWidth, bigBtnHeight)))
            {

            }
        }
        ImGui::End();
    }

}

void UIManager::RenderShaderSelectorPanel()
{
    static bool isVisable = true;
    if (isVisable)
    {
        ImGui::Begin("Select Shader", &isVisable);
        {
            int shadersCount = 0;
            const char** shaderNames = ShaderManager::getInstance()->getAllShadersName(&shadersCount);

            int currentid = -1;
            //static
            if (ImGui::ListBox("##", &currentid, shaderNames, shadersCount))
            {
                LogUtil::printInfo(std::to_string(currentid));
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
    RenderShaderSelectorPanel();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::OnFinalize()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
