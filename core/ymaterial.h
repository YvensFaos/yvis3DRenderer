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
        const std::string identifier;
        GLuint shaderProgram;
        GLenum renderMode;
        bool supportLight{};
        std::vector<std::shared_ptr<core::YUniform> > uniforms;

    public:
        YMaterial(std::string identifier, const std::string &vertexShader, const std::string &fragmentShader,
                  const std::string &geometryShader = "");

        ~YMaterial() = default;

        void drawModel(const YModel &model) const;

        [[nodiscard]] GLenum getRenderMode() const;

        void setRenderMode(GLenum mode);

        [[nodiscard]] GLuint getProgram() const;

        [[nodiscard]] std::vector<std::shared_ptr<core::YUniform> >::const_iterator getUniformsIterator() const;

        [[nodiscard]] std::vector<std::shared_ptr<core::YUniform> >::const_iterator getUniformsEndIterator() const;

        [[nodiscard]] core::YUniform* getUniform(const std::string &uniformName) const;

        std::shared_ptr<YUniform> createCustomUniform(const std::string &uniformName, YUniformType type);

        [[nodiscard]] bool doesSupportLight() const;

        void setSupportLight(bool newSupportLight);
    };
} // core
