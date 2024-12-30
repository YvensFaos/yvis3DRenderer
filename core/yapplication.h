//
// Created by Yvens Serpa on 30/12/2024.
//

#pragma once
#include <memory>

namespace core {
    class YScene;
    class YRenderer;
}

namespace core {
    class YApplication {
        std::shared_ptr<YRenderer> renderer;
        std::shared_ptr<YScene> currentScene;
    public:
        YApplication(float width, float height, std::string title);

        YApplication(const YApplication &) = delete;

        YApplication &operator=(const YApplication &) = delete;

        ~YApplication();

        void run();

    private:
        void setCurrentScene(std::shared_ptr<YScene> scene);
    };
} // core
