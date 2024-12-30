//
// Created by Yvens Serpa on 30/12/2024.
//

#pragma once
#include <memory>
#include <vector>

namespace core {
    class YScene;
    class YRenderer;
}

namespace core {
    class YApplication {
        std::shared_ptr<YRenderer> renderer;
        std::shared_ptr<YScene> currentScene;
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
        void setCurrentScene(std::shared_ptr<YScene> scene);
    };
} // core
