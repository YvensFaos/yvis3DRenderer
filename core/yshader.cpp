//
// Created by Yvens Serpa on 07/12/2024.
//

#include <vector>
#include "yshader.h"

namespace core {
    GLuint YShader::generateShader(const std::string &shaderText, GLuint shaderType) {
        printf("Loading shader:\n%s\n", shaderText.c_str());
        const GLuint shader = glCreateShader(shaderType);
        const char *s_str = shaderText.c_str();
        glShaderSource(shader, 1, &s_str, nullptr);
        glCompileShader(shader);
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, nullptr, &errorLog[0]);
            std::string errorMessage(begin(errorLog), end(errorLog));
            glDeleteShader(shader);

            printf("Shader Error:\n%s\n\n", errorMessage.c_str());
        } else {
            printf("Shader compiled:[%d].\n\n", shader);
        }
        return shader;
    }

    GLuint YShader::generateProgram(GLuint vertexShader, GLuint fragmentShader) {
        GLuint shaderProgramme = glCreateProgram();
        glAttachShader(shaderProgramme, vertexShader);
        glAttachShader(shaderProgramme, fragmentShader);
        glLinkProgram(shaderProgramme);

        printf("Program generated:[%d].\n\n", shaderProgramme);

        return shaderProgramme;
    }

    GLuint YShader::generateProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
        GLuint shaderProgramme = glCreateProgram();
        glAttachShader(shaderProgramme, vertexShader);
        glAttachShader(shaderProgramme, geometryShader);
        glAttachShader(shaderProgramme, fragmentShader);
        glLinkProgram(shaderProgramme);

        printf("Program generated:[%d].\n\n", shaderProgramme);

        return shaderProgramme;
    }

    GLuint YShader::generateProgram(const std::vector<GLuint> &shaders) {
        const GLuint shaderProgramme = glCreateProgram();
        for (const GLuint shader: shaders) {
            glAttachShader(shaderProgramme, shader);
        }
        glLinkProgram(shaderProgramme);

        printf("Program generated:[%d] [From %lu shader programs].\n\n", shaderProgramme, shaders.size());

        return shaderProgramme;
    }
} // core
