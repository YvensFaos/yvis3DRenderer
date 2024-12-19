//
// Created by Yvens Serpa on 19/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

namespace core {
    class YRenderQuad {
    protected:
        GLuint vertexShader{};
        GLuint fragmentShader{};
        GLuint programme{};

        GLuint textureUniform{};
        GLuint quadVAO{};
        GLuint quadVBO{};

        static std::string defaultVertexShader;
        static std::string defaultFragmentShader;
    public:
        YRenderQuad();
        explicit YRenderQuad(const std::string &fragmentShaderText);
        YRenderQuad(const std::string &vertexShaderText, const std::string &fragmentShaderText);
        virtual ~YRenderQuad();

        void render(GLuint texture, bool setupProgramme = true) const;
        void renderCubeMap(GLuint cubeMapTexture) const;
        void render(const std::vector<GLuint> &textures) const;
        [[nodiscard]] GLuint getProgramme() const;
    protected:
        void initialize(const std::string &vertexShaderText, const std::string &fragmentShaderText);
    };
}

