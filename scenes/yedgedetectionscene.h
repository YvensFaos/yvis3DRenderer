//
// Created by Yvens Serpa on 19/12/2024.
//

#pragma once
#include "../core/yframebuffer.h"
#include "../core/ylight.h"
#include "../core/yscene.h"
#include "../core/yrenderquad.h"
#include "../core/ymodel.h"

namespace scenes {
    class YEdgeDetectionScene final : public core::YScene {
        GLuint shaderProgram;
        core::YRenderQuad edgeQuad;
        core::YFrameBuffer edgeBuffer;
        std::vector<core::YModel> models;
        core::YLight light;
        glm::mat4 modelMatrix;
    public:
        explicit YEdgeDetectionScene(core::YRenderer &renderer, const std::string &file, int width, int height);

        YEdgeDetectionScene(const YEdgeDetectionScene &edgeDetectionScene) = delete;

        YEdgeDetectionScene &operator=(const YEdgeDetectionScene &edgeDetectionScene) = delete;

    protected:
        void renderImpl() override;
    };
}
