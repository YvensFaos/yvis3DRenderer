//
// Created by Yvens Serpa on 07/12/2024.
//

#include <vector>
#include <sstream>
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

    std::vector<std::shared_ptr<YUniform>> YShader::getUniformsFromShaderProgram(const std::string &shaderText) {
        std::vector<std::shared_ptr<YUniform>> uniforms;

        std::istringstream inputStream(shaderText);
        std::string line;

        while (std::getline(inputStream, line)) {
            int index = static_cast<int>(line.find("uniform"));
            if (index != -1) {
                std::istringstream lineStream(line);
                std::string token;
                std::vector<std::string> tokens;

                while (lineStream >> token) {
                    tokens.push_back(token);
                }

                std::string uniformName = tokens[2];
                uniformName.pop_back();
                printf("Uniform found: %s %s %s.\n", tokens[0].c_str(), tokens[1].c_str(), uniformName.c_str());

                auto type = tokens[1];
                if (type == "mat4") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::MAT4));
                } else if (type == "mat3") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::MAT3));
                } else if (type == "vec4") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::VEC4));
                } else if (type == "vec3") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::VEC3));
                } else if (type == "vec2") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::VEC2));
                } else if (type == "bool") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::BOOL));
                } else if (type == "float") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::FLOAT));
                } else if (type == "int") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::INT));
                } else if (type == "sampler2D") {
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::SAMPLER2D));
                } else {
                    printf("Custom uniform type found: %s\n.", type.c_str());
                    uniforms.emplace_back(std::make_shared<YUniform>(uniformName, YUniformType::CUSTOM));
                }
            }
        }

        return uniforms;
    }
} // core
