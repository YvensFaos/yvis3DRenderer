//
// Created by Yvens Serpa on 16/01/2025.
//

#include "yfoguniform.h"

#include <string>

#include "../../elements/yfog.h"

namespace core::uniforms {
    YFogUniform::YFogUniform(const GLuint shaderProgramme, const std::shared_ptr<elements::YFog> &fog, const std::shared_ptr<core::YUniform>& uniform): YBaseUniformValue(uniform), fog(fog) {
        static char fogUniformName[32];
        snprintf(fogUniformName, 32, "sceneFog.");
        std::string buffer;

        auto getFogUniformLocation = [](std::string &stringBuffer, char uniName[64], GLuint &uniformPosition,
                                          const GLuint shader, const std::string &uniformProperty) {
            stringBuffer = "";
            stringBuffer.append(uniName);
            uniformPosition = glGetUniformLocation(shader, stringBuffer.append(uniformProperty).c_str());
        };

        getFogUniformLocation(buffer, fogUniformName, minDistanceUniform, shaderProgramme, "minDist");
        getFogUniformLocation(buffer, fogUniformName, maxDistanceUniform, shaderProgramme, "maxDist");
        getFogUniformLocation(buffer, fogUniformName, fogColorUniform, shaderProgramme, "color");
    }

    void YFogUniform::stream() const {
        const auto minDistance = fog->minDist;
        const auto maxDistance = fog->maxDist;
        const auto color = fog->color;

        glUniform1f(static_cast<GLint>(minDistanceUniform), minDistance);
        glUniform1f(static_cast<GLint>(maxDistanceUniform), maxDistance);
        glUniform4f(static_cast<GLint>(fogColorUniform), color.x, color.y, color.z, color.w);
    }

    void YFogUniform::updateValue(const YBaseUniformValue &another) {
        if (const auto derived = dynamic_cast<const YFogUniform*>(&another)) {
            fog = derived->getFog();
        }
    }

    std::shared_ptr<elements::YFog> YFogUniform::getFog() const {
        return fog;
    }
} // core