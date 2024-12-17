//
// Created by Yvens Serpa on 09/12/2024.
//

#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/fwd.hpp>

namespace elements {
    class YSkybox {
        GLuint cubeMapTextureID;
        GLint width;
        GLint height;
        GLint channels;
        GLuint VAO;
        GLuint VBO;

        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint programme;
        GLint viewProjectionUniform;
        GLint skyboxUniform;

        std::vector<std::string> faces;
        static std::string defaultVertexShader;
        static std::string defaultFragmentShader;

    public:
        explicit YSkybox(const std::vector<std::string> &faces);

        ~YSkybox() = default;

        void render(glm::mat4 viewProjectionMatrix) const;
    };
} // elements
