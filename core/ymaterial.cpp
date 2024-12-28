//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "ymaterial.h"

#include "../core/yshader.h"

namespace core {
    YMaterial::YMaterial(std::string identifier, const std::string &vertexShader, const std::string &fragmentShader,
                         const std::string &geometryShader) : identifier(std::move(identifier)), shaderProgram(-1),
                                                              renderMode(GL_TRIANGLES) {
        const auto vertexProgram = core::YShader::generateShader(vertexShader, GL_VERTEX_SHADER);
        const auto fragmentProgram = core::YShader::generateShader(fragmentShader, GL_FRAGMENT_SHADER);

        const auto hasGeometryShader = !geometryShader.empty();

        if (!hasGeometryShader) {
            shaderProgram = core::YShader::generateProgram(vertexProgram, fragmentProgram);
        } else {
            auto geometryProgram = core::YShader::generateShader(geometryShader, GL_GEOMETRY_SHADER);
            shaderProgram = core::YShader::generateProgram(vertexProgram, geometryProgram, fragmentProgram);
        }

        auto vertexShaderUniforms = core::YShader::getUniformsFromShaderProgram(vertexShader);
        auto fragmentShaderUniforms = core::YShader::getUniformsFromShaderProgram(fragmentShader);

        uniforms.insert(uniforms.end(), vertexShaderUniforms.begin(), vertexShaderUniforms.end());
        uniforms.insert(uniforms.end(), fragmentShaderUniforms.begin(), fragmentShaderUniforms.end());

        if (hasGeometryShader) {
            auto geometryShaderUniforms = core::YShader::getUniformsFromShaderProgram(geometryShader);
            uniforms.insert(uniforms.end(), geometryShaderUniforms.begin(), geometryShaderUniforms.end());
        }

        std::ranges::sort(uniforms);
        const auto last = std::ranges::unique(uniforms, [](const std::shared_ptr<YUniform> &uniformA,
                                                           const std::shared_ptr<YUniform> &uniformB) {
            return uniformA->uniformName == uniformB->uniformName;
        }).begin();
        uniforms.erase(last, uniforms.end());
        std::ranges::sort(uniforms);

        for (const auto &uniform: uniforms) {
            const GLuint uniformLocation = glGetUniformLocation(shaderProgram, uniform->uniformName.c_str());
            if (const GLenum error = glGetError(); error != GL_NO_ERROR) {
                printf("Error: %d!\n", error);
            }

            uniform->uniformLocation = static_cast<int>(uniformLocation);
        }
    }

    void YMaterial::drawModel(const YModel &model) const {
        model.draw(shaderProgram, renderMode);
    }

    void YMaterial::setRenderMode(const GLenum mode) {
        renderMode = mode;
    }

    std::vector<std::shared_ptr<core::YUniform>>::const_iterator YMaterial::getUniformsIterator() const {
        return uniforms.cbegin();
    }

    std::vector<std::shared_ptr<core::YUniform>>::const_iterator YMaterial::getUniformsEndIterator() const {
        return uniforms.cend();
    }

    bool YMaterial::doesSupportLight() const {
        return supportLight;
    }

    void YMaterial::setSupportLight(bool newSupportLight) {
        supportLight = newSupportLight;
    }

    GLuint YMaterial::getProgram() const {
        return shaderProgram;
    }
} // core