//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "yuniform.h"
#include "ymodel.h"

namespace core {

    class YMaterial {
    private:
        const std::string identifier;
        GLuint shaderProgram;
        std::vector<std::shared_ptr<core::YUniform>> uniforms;
        GLenum renderMode;

    public:
        YMaterial(std::string identifier, const std::string &vertexShader, const std::string &fragmentShader,
                  const std::string &geometryShader = "");

        ~YMaterial() = default;

        void drawModel(const YModel &model) const;

        void setRenderMode(GLenum mode);

        [[nodiscard]] GLuint getProgram() const;

        [[nodiscard]] std::vector<std::shared_ptr<core::YUniform>>::const_iterator getUniformsIterator() const;

        [[nodiscard]] std::vector<std::shared_ptr<core::YUniform>>::const_iterator getUniformsEndIterator() const;
    };

} // core
