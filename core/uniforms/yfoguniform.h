//
// Created by Yvens Serpa on 16/01/2025.
//

#pragma once
#include <memory>
#include <GL/glew.h>

#include "../ybaseuniformvalue.h"

namespace elements {
    struct YFog;
}

namespace core::uniforms {
    struct YFogUniform final : YBaseUniformValue {
        GLuint minDistanceUniform{};
        GLuint maxDistanceUniform{};
        GLuint fogColorUniform{};
        std::shared_ptr<elements::YFog> fog;

        explicit YFogUniform(GLuint shaderProgramme, const std::shared_ptr<elements::YFog>& fog, const std::shared_ptr<core::YUniform>& uniform);
        void stream() const override;
        void updateValue(const YBaseUniformValue & another) override;
        std::shared_ptr<elements::YFog> getFog() const;
    };
}
