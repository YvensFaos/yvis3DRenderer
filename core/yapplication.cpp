//
// Created by Yvens Serpa on 30/12/2024.
//

#include "yapplication.h"

#include <string>

#include "yrenderer.h"
#include "yscene.h"
#include "imgui.h"
#include "../scenes/yloadedscene.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace core {
    YApplication::YApplication(float width, float height, std::string title) {
        renderer = std::make_unique<YRenderer>(width, height, title);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(renderer->getWindow().get(), true);
        ImGui_ImplOpenGL3_Init("#version 400");
    }

    YApplication::~YApplication() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void YApplication::run() {
        // Render
        do {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("YApplication");
            static char sceneName[196] = "data/scenes/loaded_scene_example.lua";
            auto sceneTextName = currentScene != nullptr ? std::string(sceneName) : "No scene loaded.";
            ImGui::Text("%s", sceneTextName.c_str());
            ImGui::InputTextWithHint("Scene to be loaded:", "", sceneName, IM_ARRAYSIZE(sceneName));
            if(ImGui::Button("Load Example Scene")) {
                currentScene = std::make_shared<scenes::YLoadedScene>(*renderer.get(), sceneName, renderer->getWidth(), renderer->getHeight());
            }

            ImGui::End();

            ImGui::Render();
            if(currentScene != nullptr) {
                currentScene->render();
            } else {
                renderer->startFrame();
            }
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            renderer->finishFrame();

        } while (renderer->isRunning());
        renderer->closeRenderer();
    }

    void YApplication::setCurrentScene(std::shared_ptr<YScene> scene) {
        currentScene = scene;
    }
} // core