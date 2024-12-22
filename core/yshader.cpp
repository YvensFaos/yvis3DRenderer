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

    std::vector<YUniform> YShader::getUniformsFromShaderProgram(const std::string& shaderText) {
        std::vector<YUniform> uniforms;

        std::istringstream inputStream(shaderText);
        std::string line;

        while (std::getline(inputStream, line)) {
            if (line.find("uniform") == 0) {
                std::istringstream lineStream(line);
                std::string token;
                std::vector<std::string> tokens;

                while (lineStream >> token) {
                    tokens.push_back(token);
                }

                printf("Uniform found: %s %s %s\n.", tokens[0].c_str(), tokens[1].c_str(), tokens[2].c_str());
                auto type = tokens[1];
                if(type == "mat4") {
                    uniforms.push_back({tokens[1], YUniformType::MAT4});
                } else if (type == "mat3") {
                    uniforms.push_back({tokens[1], YUniformType::MAT3});
                } else if (type == "vec4") {
                    uniforms.push_back({tokens[1], YUniformType::VEC4});
                } else if (type == "vec3") {
                    uniforms.push_back({tokens[1], YUniformType::VEC3});
                } else if (type == "vec2") {
                    uniforms.push_back({tokens[1], YUniformType::VEC2});
                } else if (type == "bool") {
                    uniforms.push_back({tokens[1], YUniformType::BOOL});
                } else if (type == "float") {
                    uniforms.push_back({tokens[1], YUniformType::FLOAT});
                } else if (type == "int") {
                    uniforms.push_back({tokens[1], YUniformType::INT});
                } else if (type == "sampler2D") {
                    uniforms.push_back({tokens[1], YUniformType::SAMPLER2D});
                } else {
                    printf("Custom uniform type found: %s\n.", type.c_str());
                    uniforms.push_back({tokens[1], YUniformType::CUSTOM});
                }
            }
        }

        return uniforms;
    }
} // core
