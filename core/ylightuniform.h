//
// Created by Yvens Serpa on 28/12/2024.
//

#pragma once
#include <memory>
#include <GL/glew.h>

#include "ybaseuniformvalue.h"

namespace core {

class YLight;
struct YUniform;

struct YLightUniform final : YBaseUniformValue {
    int lightIndex;
    GLuint lightPositionUniform{};
    GLuint lightDirectionUniform{};
    GLuint lightColorUniform{};
    GLuint lightIntensityUniform{};
    GLuint lightSpecularUniform{};
    GLuint lightDirectionalUniform{};
    std::shared_ptr<core::YLight> light;

    explicit YLightUniform(int lightIndex, GLuint shaderProgramme, const std::shared_ptr<core::YLight>& light, const std::shared_ptr<core::YUniform>& uniform);

    void stream() const override;
};

} // core
