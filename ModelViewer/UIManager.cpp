#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>


#include "UIManager.h"
#include "EngineManager.h"

#include "Utils/LogUtil.hpp"

auto clickEvent = std::make_shared<Event>("ClickEvent", "hello world");
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

    EngineManager::getInstance()->initNewTrigger(clickEvent, testTrigger);
}

void UIManager::OnRenderUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))
        {
            EngineManager::getInstance()->tryTriggerEvent(clickEvent);
            counter++;
        }

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

