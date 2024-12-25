//
// Created by Yvens Serpa on 09/12/2024.
//

#pragma once

#include <GL/glew.h>

#include "../core/yscene.h"
#include "../core/ymodel.h"
#include "../core/ylight.h"
#include "../elements/yfog.h"

namespace scenes {
    class YFogScene final : public core::YScene {
        GLuint shaderProgram;
        core::YModel monkey;
        core::YLight light;
        elements::YFog fog;
        glm::mat4 modelMatrix;

    public:
        explicit YFogScene(core::YRenderer &renderer, const std::string &file, int width, int height);

        YFogScene(const YFogScene &fogScene) = delete;

        YFogScene &operator=(const YFogScene &fogScene) = delete;

    protected:
        void renderImpl() override;
    };
} // scenes
