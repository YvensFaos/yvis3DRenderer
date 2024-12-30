//
// Created by Yvens Serpa on 30/12/2024.
//

#include "yapplication.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "yrenderer.h"
#include "yscene.h"
#include "imgui.h"
#include "../scenes/yloadedscene.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace core {
    YApplication::YApplication(float width, float height, const std::string &title) : maxFrames(100),
        currentFramesIndex(0) {
        renderer = std::make_unique<YRenderer>(width, height, title);

        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(renderer->getWindow().get(), true);
        ImGui_ImplOpenGL3_Init("#version 400");

        frames.resize(maxFrames);
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
            static char fpsLabel[32];
            snprintf(fpsLabel, 32, "FPS [%6.3f]", renderer->getFPS());
            ImGui::PlotLines(fpsLabel, &frames[0], maxFrames, 0, nullptr, 0.0f, 100.0f, ImVec2(300, 30));
            static char sceneName[196] = "data/scenes/loaded_scene_example.lua";
            auto sceneTextName = currentScene != nullptr ? std::string(sceneName) : "No scene loaded.";
            ImGui::Text("%s", sceneTextName.c_str());
            ImGui::InputTextWithHint("Scene", "", sceneName, IM_ARRAYSIZE(sceneName));
            if (ImGui::Button("Load Example Scene")) {
                currentScene = std::make_shared<scenes::YLoadedScene>(*renderer, sceneName, renderer->getWidth(),
                                                                      renderer->getHeight());
                if (!currentScene->isLoaded()) {
                    currentScene = nullptr;
                }
            }
            ImGui::End();
            const bool loadedScene = currentScene != nullptr;

            if (loadedScene) {
                ImGui::Begin("Camera");
                const auto camera = renderer->getCamera();
                auto pos = camera.getPos();
                auto dir = camera.getDir();
                ImGui::InputFloat3("Pos", glm::value_ptr(pos));
                ImGui::InputFloat3("Dir", glm::value_ptr(dir));
                if (ImGui::Button("Print to Console")) {
                    camera.logToConsole();
                }
                ImGui::End();
            }

            ImGui::Render();
            if (loadedScene) {
                currentScene->render();
            } else {
                renderer->startFrame();
            }
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            renderer->finishFrame();
            frames[currentFramesIndex] = renderer->getFPS();
            currentFramesIndex = (currentFramesIndex + 1) % maxFrames;
        } while (renderer->isRunning());
        renderer->closeRenderer();
    }

    void YApplication::setCurrentScene(std::shared_ptr<YScene> scene) {
        currentScene = std::move(scene);
    }
} // core
