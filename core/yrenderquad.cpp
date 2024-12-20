//
// Created by Yvens Serpa on 19/12/2024.
//

#include "yrenderquad.h"

#include <GL/glew.h>

#include "yshader.h"


float quadVertices[] = {
    -1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, 0.0f, 1.0f, 0.0f,
};

std::string core::YRenderQuad::defaultVertexShader =
        "#version 400\n"
        "    layout (location = 0) in vec3 vertex;\n"
        "    layout (location = 1) in vec2 uv;\n"
        "    out vec2 vuv;\n"
        "    void main()\n"
        "    {\n"
        "        vuv = uv;\n"
        "        gl_Position = vec4(vertex, 1.0);\n"
        "    }\n";

std::string core::YRenderQuad::defaultFragmentShader =
        "#version 400\n"
        "   in vec2 vuv;\n"
        "   uniform sampler2D textureUniform;\n"
        "   out vec4 frag_colour;\n"
        "   void main()\n"
        "   {\n"
        "       vec4 value = texture(textureUniform, vuv);\n"
        "       frag_colour = vec4(vec3(value), 1.0);\n"
        "   }\n";

core::YRenderQuad::YRenderQuad() : fragmentShader(0) {
    initialize(defaultVertexShader, defaultFragmentShader);
}

core::YRenderQuad::YRenderQuad(const std::string &fragmentShaderText) {
    initialize(defaultVertexShader, fragmentShaderText);
}

core::YRenderQuad::YRenderQuad(const std::string &vertexShaderText, const std::string &fragmentShaderText) {
    initialize(vertexShaderText, fragmentShaderText);
}

core::YRenderQuad::~YRenderQuad() {
    glDeleteProgram(programme);
}

void core::YRenderQuad::render(const GLuint texture, const bool setupProgramme) const {
    if (setupProgramme) {
        glUseProgram(programme);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(static_cast<GLint>(textureUniform), 0);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void core::YRenderQuad::renderCubeMap(const GLuint cubeMapTexture) const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glUniform1i(static_cast<GLint>(textureUniform), 0);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
}

void core::YRenderQuad::render(const std::vector<GLuint> &textures) const {
    for (size_t i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        std::string textureName("textureUniform" + std::to_string(i));
        glUniform1i(glGetUniformLocation(programme, textureName.c_str()), static_cast<GLint>(i));
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void core::YRenderQuad::initialize(const std::string &vertexShaderText, const std::string &fragmentShaderText) {
    vertexShader = YShader::generateShader(vertexShaderText, GL_VERTEX_SHADER);
    fragmentShader = YShader::generateShader(fragmentShaderText, GL_FRAGMENT_SHADER);
    programme = YShader::generateProgram(vertexShader, fragmentShader);
    textureUniform = glGetUniformLocation(programme, "textureUniform");

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
}

GLuint core::YRenderQuad::getProgramme() const {
    return this->programme;
}
