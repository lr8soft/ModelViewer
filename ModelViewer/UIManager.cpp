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

#include "Utils/FileOpenUtil.h"
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
                bool bResult = FileOpenUtil::openNewFile(filePath);
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
    static char vertexShader[260] = { 0 };
    static char fragShader[260] = { 0 };
    static int selectedShaderIndex = -1;

    static bool isVisable = true;
    if (isVisable)
    {
        ImGui::Begin("Select Shader", &isVisable);
        {
            ImGui::Text("Current Shader:");
            ImGui::SameLine();
            ImGui::Text(ShaderManager::getInstance()->getCurrentShaderName().c_str());

            int shadersCount = 0;
            const char** shaderNames = ShaderManager::getInstance()->getAllShadersName(&shadersCount);

            ImGui::Text("Available shaders:");
            if (ImGui::ListBox("##", &selectedShaderIndex, shaderNames, shadersCount))
            {
                LogUtil::printInfo(std::to_string(selectedShaderIndex));
            }

            ImGui::SameLine();
            if (ImGui::Button("Apply shader"))
            {

            }

            if (ImGui::TreeNode("Open New Shader"))
            {
                {
                    if (ImGui::Button("Select Vertex Shader"))
                    {
                        FileOpenUtil::openNewFile(vertexShader, "Open Vertex Shader", "Vertex Shader(*.vert)\0*.vert\0All Files(*.*)\0*.*\0\0");
                    }
                    ImGui::SameLine();
                    ImGui::InputText("Vertex Shader", vertexShader, 255);
                }
                {
                    if (ImGui::Button("Select Fragment Shader"))
                    {
                        FileOpenUtil::openNewFile(fragShader, "Open Fragment Shader", "Fragment Shader(*.frag)\0*.frag\0All Files(*.*)\0*.*\0\0");
                    }
                    ImGui::SameLine();
                    ImGui::InputText("Fragment Shader", fragShader, 255);
                }

                if (ImGui::Button("Load Shader")) 
                {
                
                }
                ImGui::TreePop();
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
