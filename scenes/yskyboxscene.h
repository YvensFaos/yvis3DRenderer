//
// Created by Yvens Serpa on 17/12/2024.
//

#pragma once

#include "../core/yscene.h"
#include "../core/ymodel.h"
#include "../elements/yskybox.h"

namespace scenes {
    class YSkyBoxScene final : public core::YScene {
        elements::YSkybox skybox;
        glm::mat4 viewProjectionMatrix{};

    public:
        explicit YSkyBoxScene(core::YRenderer &renderer, const std::string &file, const std::string &skyboxTable,
                              int width, int height);

        YSkyBoxScene(const YSkyBoxScene &fogScene) = delete;

        YSkyBoxScene &operator=(const YSkyBoxScene &fogScene) = delete;

    protected:
        void renderImpl() override;
    };
}
