//
// Created by Yvens Serpa on 19/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

#include "yrenderquad.h"

namespace core {
    class YCustomRenderQuad final : public YRenderQuad {
    protected:
        GLuint vertexShader{};
        GLuint fragmentShader{};
        GLuint programme{};

        GLuint textureUniform{};

        static std::string defaultVertexShader;
        static std::string defaultFragmentShader;

    public:
        YCustomRenderQuad();

        explicit YCustomRenderQuad(const std::string &fragmentShaderText);

        YCustomRenderQuad(const std::string &vertexShaderText, const std::string &fragmentShaderText);

        ~YCustomRenderQuad() override;

        void render() const override;

        [[nodiscard]] GLuint getProgramme() const;

    private:
        void generateProgram(const std::string &vertexShaderText, const std::string &fragmentShaderText);
    };
}
