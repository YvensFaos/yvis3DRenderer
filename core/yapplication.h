//
// Created by Yvens Serpa on 30/12/2024.
//

#pragma once
#include <memory>
#include <vector>

namespace core {
    class YRenderer;
    class YRenderQuad;
    class YFrameBuffer;

}

namespace scenes {
    class YLoadedScene;
}

namespace core {
    class YApplication {
        std::shared_ptr<YRenderer> renderer;
        std::shared_ptr<scenes::YLoadedScene> currentScene;
        std::shared_ptr<YFrameBuffer> sceneFrameBuffer;
        std::shared_ptr<YRenderQuad> sceneRenderQuad;
        std::vector<float> frames;
        int maxFrames;
        int currentFramesIndex;

    public:
        YApplication(float width, float height, const std::string& title);

        YApplication(const YApplication &) = delete;

        YApplication &operator=(const YApplication &) = delete;

        ~YApplication();

        void run();

    private:
        void setCurrentScene(std::shared_ptr<scenes::YLoadedScene> scene);
    };
} // core
