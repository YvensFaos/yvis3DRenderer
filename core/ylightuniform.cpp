//
// Created by Yvens Serpa on 28/12/2024.
//

#include "ylightuniform.h"

#include "ylight.h"
#include "yuniform.h"

namespace core {
    YLightUniform::YLightUniform(const int lightIndex, const GLuint shaderProgramme, const std::shared_ptr<core::YLight> &light,
        const std::shared_ptr<core::YUniform>& uniform):
    YBaseUniformValue(uniform), lightIndex(lightIndex), light(light) {
        char uniformName[64];
        snprintf(uniformName, 64, "%s.", uniform->uniformName.c_str());
        std::string buffer;

        auto getLightUniformLocation = [](std::string &stringBuffer, char uniName[64], GLuint &uniformPosition,
                                          const GLuint shader, const std::string &uniformProperty) {
            stringBuffer = "";
            stringBuffer.append(uniName);
            uniformPosition = glGetUniformLocation(shader, stringBuffer.append(uniformProperty).c_str());
        };

        getLightUniformLocation(buffer, uniformName, lightPositionUniform, shaderProgramme, "position");
        getLightUniformLocation(buffer, uniformName, lightDirectionUniform, shaderProgramme, "direction");
        getLightUniformLocation(buffer, uniformName, lightColorUniform, shaderProgramme, "color");
        getLightUniformLocation(buffer, uniformName, lightIntensityUniform, shaderProgramme, "intensity");
        getLightUniformLocation(buffer, uniformName, lightDirectionalUniform, shaderProgramme, "directional");
        getLightUniformLocation(buffer, uniformName, lightSpecularUniform, shaderProgramme, "specularPower");
    }

    void YLightUniform::stream() const {
        const auto position = light->getPosition();
        const auto direction = light->getDirection();
        const auto color = light->getColor();
        const auto intensity = light->getIntensity();
        const auto directional = light->getDirectional();
        const auto specularPower = light->getSpecularPower();

        glUniform3f(static_cast<GLint>(lightPositionUniform), position.x, position.y, position.z);
        glUniform3f(static_cast<GLint>(lightDirectionUniform), direction.x, direction.y, direction.z);
        glUniform4f(static_cast<GLint>(lightColorUniform), color.x, color.y, color.z, color.w);
        glUniform1f(static_cast<GLint>(lightIntensityUniform), intensity);
        glUniform1i(static_cast<GLint>(lightDirectionalUniform), directional);
        glUniform1f(static_cast<GLint>(lightSpecularUniform), specularPower);
    }

    void YLightUniform::updateValue(const YBaseUniformValue &another) {
        if (const auto derived = dynamic_cast<const YLightUniform*>(&another)) {
            light = derived->getLight();
        }
    }

    std::shared_ptr<core::YLight> YLightUniform::getLight() const {
        return light;
    }
} // core