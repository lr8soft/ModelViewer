#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

#include <Windows.h>
#include <commdlg.h>

#include "UIManager.h"
#include "RenderData.h"
#include "RenderManager.h"
#include "PublicRenderData.h"
#include "LogicalManager.h"
#include "ShaderManager.h"

#include "Utils/InteractionUtil.h"
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

    static int selectedModelIndex = -1;

    int bigBtnWidth = 100, bigBtnHeight = 30;
    if (isVisable)
    {
        ImGui::Begin("Model Window", &isVisable);
        {
            // Tab Open new model
            if(ImGui::TreeNode("Open New Model..."))
            {
                if (ImGui::Button("Select..."))
                {
                    InteractionUtil::openNewFile(filePath);
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
                    UniformData mvpMatrix;
                    mvpMatrix.attrName = "model";
                    mvpMatrix.value.matrix5 = matrix;
                    mvpMatrix.valueIndex = 5;

                    RenderManager::getInstance()->tryTriggerEvent(EVENT_SEND_UNIFORM_DATA, std::make_shared<UniformData>(mvpMatrix));
                    RenderManager::getInstance()->tryTriggerEvent(EVENT_SEND_UNIFORM_CAMERA_DATA, nullptr, true);

                    RenderData data;
                    data.modelName = filePath;
                    RenderManager::getInstance()->tryTriggerEvent(EVENT_RENDER_MODEL, std::make_shared<RenderData>(data), true);
                }

                ImGui::SameLine();
                if (ImGui::Button("ClearBtn", ImVec2(bigBtnWidth, bigBtnHeight)))
                {

                }
                ImGui::TreePop();
            }
            // Tab current model list
            if (ImGui::TreeNode("Scene Collection"))
            {
                int modelsCount = 0;
                const char** modelsName = PublicRenderData::getAllRenderingModels(&modelsCount);
                ImGui::ListBox("##", &selectedModelIndex, modelsName, modelsCount);

                ImGui::SameLine();
                // Delete model
                if (ImGui::Button("Delete Model"))
                {
                    if (selectedModelIndex >= 0)
                    {
                        const char* targetModelName = *(modelsName + selectedModelIndex);
                        RenderManager::getInstance()->tryTriggerEvent(EVENT_STOP_RENDER_MODEL, targetModelName);
                    }
                }

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

}

void UIManager::RenderShaderSelectorPanel()
{
    static char newShaderName[20] = { 0 };
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
            // show all available shaders
            ImGui::ListBox("##", &selectedShaderIndex, shaderNames, shadersCount);

            ImGui::SameLine();
            if (ImGui::Button("Apply shader"))
            {
                if (selectedShaderIndex >= 0)
                {
                    const char* targetShaderName = *(shaderNames + selectedShaderIndex);

                    static ShaderData shader;
                    shader.shaderName = targetShaderName;

                    RenderManager::getInstance()->tryTriggerEvent(EVENT_LOAD_SHADER, &shader);
                }
            }

            if (ImGui::TreeNode("Open New Shader"))
            {
                {
                    ImGui::InputText("Shader  Name", newShaderName, IM_ARRAYSIZE(newShaderName));
                }
                {
                    if (ImGui::Button("Select VS"))
                    {
                        InteractionUtil::openNewFile(vertexShader, "Open Vertex Shader", "Vertex Shader(*.vert)\0*.vert\0All Files(*.*)\0*.*\0\0");
                    }
                    ImGui::SameLine();
                    ImGui::InputText("Vertex Shader", vertexShader, IM_ARRAYSIZE(vertexShader));
                }
                {
                    if (ImGui::Button("Select FS"))
                    {
                        InteractionUtil::openNewFile(fragShader, "Open Fragment Shader", "Fragment Shader(*.frag)\0*.frag\0All Files(*.*)\0*.*\0\0");
                    }
                    ImGui::SameLine();
                    ImGui::InputText("Fragment Shader", fragShader, IM_ARRAYSIZE(fragShader));
                }

                if (ImGui::Button("Load Shader")) 
                {
                    do 
                    {
                        if (strlen(newShaderName) == 0)
                        {
                            InteractionUtil::showMessageBox("No shader name!", "Error");
                            break;
                        }

                        if (strlen(vertexShader) == 0)
                        {
                            InteractionUtil::showMessageBox("No vertex shader!", "Error");
                            break;
                        }

                        if (strlen(fragShader) == 0)
                        {
                            InteractionUtil::showMessageBox("No fragment shader!", "Error");
                            break;
                        }

                        static ShaderData newShader;
                        newShader.shaderName = newShaderName;
                        newShader.vertexShader = vertexShader;
                        newShader.fragShader = fragShader;

                        RenderManager::getInstance()->tryTriggerEvent(EVENT_LOAD_SHADER, &newShader);
                    } while (0);

                }
                ImGui::SameLine();
                if (ImGui::Button("Clear"))
                {
                    memset(newShaderName, 0, sizeof(newShaderName));
                    memset(vertexShader, 0, sizeof(vertexShader));
                    memset(fragShader, 0, sizeof(fragShader));
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
