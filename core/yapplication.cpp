//
// Created by Yvens Serpa on 30/12/2024.
//

#include "yapplication.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "yrenderer.h"
#include "imgui.h"
#include "yframebuffer.h"
#include "yrenderquad.h"
#include "../scenes/yloadedscene.h"
#include "../view/yobjectui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../elements/ylightobject.h"

namespace core {
    float YApplication::fps = 0.0f;
    float YApplication::deltaTime = 0.0f;

    YApplication::YApplication(float width, float height, const std::string &title) : maxFrames(100),
        currentFramesIndex(0) {
        renderer = std::make_shared<YRenderer>(width, height, title);
        sceneFrameBuffer = std::make_shared<YFrameBuffer>(width * 2, height * 2);
        sceneRenderQuad = std::make_shared<YRenderQuad>();

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
                } else {
                    viewObjects.clear();
                    auto iterator = currentScene->objectsIterator();
                    auto endIterator = currentScene->objectsEnd();

                    while (iterator != endIterator) {
                        viewObjects.push_back(std::make_shared<view::YObjectUI>(*iterator));
                        ++iterator;
                    }
                    auto lightIterator = currentScene->lightsIterator();
                    auto lightEndIterator = currentScene->lightsEnd();

                    auto index = 1;
                    char lightObjectLabel[32];
                    applicationObjects.clear();
                    while (lightIterator != lightEndIterator) {
                        snprintf(lightObjectLabel, 32, "lightObject[%i]", index++);
                        applicationObjects.push_back(
                            std::make_shared<elements::YLightObject>(lightObjectLabel, *lightIterator));
                        ++lightIterator;
                    }
                }
            }
            ImGui::End();

            const bool loadedScene = currentScene != nullptr;

            if (loadedScene) {
                ImGui::Begin(currentScene->getFileName().c_str());

                for (const auto &viewObject: viewObjects) {
                    ImGui::PushID(viewObject->getIdentifier().c_str());
                    viewObject->render();
                    ImGui::PopID();
                }

                ImGui::SeparatorText("Camera");
                const auto camera = renderer->getCamera();
                auto pos = camera->getPos();
                auto dir = camera->getDir();
                if(ImGui::InputFloat3("Pos", glm::value_ptr(pos))) {
                    camera->setPos(pos);
                }
                if(ImGui::InputFloat3("Dir", glm::value_ptr(dir))) {
                    camera->setDir(dir);
                }
                if (ImGui::Button("Print to Console")) {
                    camera->logToConsole();
                }
                ImGui::End();
            }

            ImGui::Render();

            if (loadedScene) {
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);

                currentScene->render(sceneFrameBuffer->getFBO());

                for (const auto &applicationObject: applicationObjects) {
                    applicationObject->update();
                    applicationObject->draw(*renderer);
                }
                sceneFrameBuffer->unbindBuffer();

                glDisable(GL_DEPTH_TEST);
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glDisable(GL_CULL_FACE);

                sceneRenderQuad->render(sceneFrameBuffer->getFramebufferTexture());
            } else {
                renderer->startFrame();
            }

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            renderer->finishFrame();
            frames[currentFramesIndex] = renderer->getFPS();
            fps = renderer->getFPS();
            deltaTime = renderer->getDeltaTime();
            currentFramesIndex = (currentFramesIndex + 1) % maxFrames;
        } while (renderer->isRunning());
        renderer->closeRenderer();
    }

    float YApplication::getFPS() {
        return fps;
    }

    float YApplication::getDeltaTime() {
        return deltaTime;
    }

    void YApplication::setCurrentScene(std::shared_ptr<scenes::YLoadedScene> scene) {
        currentScene = std::move(scene);
    }
} // core
