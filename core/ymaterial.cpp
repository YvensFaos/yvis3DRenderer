//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "ymaterial.h"

#include <utility>
#include "../core/yshader.h"

namespace core {
    YMaterial::YMaterial(std::string  identifier, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) : identifier(std::move(identifier)), shaderProgram(-1), renderMode(GL_TRIANGLES)
    {
        auto vertexProgram = core::YShader::generateShader(vertexShader, GL_VERTEX_SHADER);
        auto fragmentProgram = core::YShader::generateShader(fragmentShader, GL_FRAGMENT_SHADER);

        auto hasGeometryShader = !geometryShader.empty();

        if(hasGeometryShader) {
            shaderProgram = core::YShader::generateProgram(vertexProgram, fragmentProgram);
        } else {
            auto geometryProgram = core::YShader::generateShader(geometryShader, GL_GEOMETRY_SHADER);
            shaderProgram = core::YShader::generateProgram(vertexProgram, geometryProgram, fragmentProgram);
        }

        auto vertexShaderUniforms = core::YShader::getUniformsFromShaderProgram(vertexShader);
        auto fragmentShaderUniforms = core::YShader::getUniformsFromShaderProgram(fragmentShader);

        uniforms.insert(uniforms.end(), vertexShaderUniforms.begin(), vertexShaderUniforms.end());
        uniforms.insert(uniforms.end(), fragmentShaderUniforms.begin(), fragmentShaderUniforms.end());

        if(hasGeometryShader) {
            auto geometryShaderUniforms = core::YShader::getUniformsFromShaderProgram(geometryShader);
            uniforms.insert(uniforms.end(), geometryShaderUniforms.begin(), geometryShaderUniforms.end());
        }

        std::sort(uniforms.begin(), uniforms.end());

        auto last = std::unique(uniforms.begin(), uniforms.end(), [](const YUniform& uniformA, const YUniform& uniformB) {
            return uniformA.uniformName == uniformB.uniformName;
        });

        std::sort(uniforms.begin(), uniforms.end());

        for (auto uniform: uniforms) {
            //TODO fix not working!
            auto uniformLocation = glGetUniformLocation(shaderProgram, uniform.uniformName.c_str());
            uniform.uniformLocation = uniformLocation;
        }
    }

    void YMaterial::drawModel(const YModel &model) const {
        model.draw(shaderProgram, renderMode);
    }

    void YMaterial::setRenderMode(const GLenum mode) {
        renderMode = mode;
    }

    std::vector<core::YUniform>::const_iterator YMaterial::getUniformsIterator() const {
        return uniforms.cbegin();
    }

    std::vector<core::YUniform>::const_iterator YMaterial::getUniformsEndIterator() const {
        return uniforms.cend();
    }
} // core