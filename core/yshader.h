//
// Created by Yvens Serpa on 07/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include "yuniform.h"

namespace core {
    class YShader {
    public:
        static GLuint generateShader(const std::string &shaderText, GLuint shaderType);

        static GLuint generateProgram(GLuint vertexShader, GLuint fragmentShader);

        static GLuint generateProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);

        static GLuint generateProgram(const std::vector<GLuint> &shaders);

        static std::vector<std::shared_ptr<YUniform>> getUniformsFromShaderProgram(const std::string &shaderText);
    };
} // core
