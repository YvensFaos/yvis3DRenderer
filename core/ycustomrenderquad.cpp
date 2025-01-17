//
// Created by Yvens Serpa on 19/12/2024.
//

#include "ycustomrenderquad.h"

#include <GL/glew.h>

#include "yshader.h"
#include "ytexture.h"

std::string core::YCustomRenderQuad::defaultVertexShader =
        "#version 400\n"
        "    layout (location = 0) in vec3 vertex;\n"
        "    layout (location = 1) in vec2 uv;\n"
        "    out vec2 vuv;\n"
        "    void main()\n"
        "    {\n"
        "        vuv = uv;\n"
        "        gl_Position = vec4(vertex, 1.0);\n"
        "    }\n";

std::string core::YCustomRenderQuad::defaultFragmentShader =
        "#version 400\n"
        "   in vec2 vuv;\n"
        "   uniform sampler2D textureUniform;\n"
        "   out vec4 frag_colour;\n"
        "   void main()\n"
        "   {\n"
        "       vec4 value = texture(textureUniform, vuv);\n"
        "       frag_colour = vec4(value.x, value.y, value.z, 1.0);\n"
        "   }\n";

core::YCustomRenderQuad::YCustomRenderQuad() : YRenderQuad() {
    generateProgram(defaultVertexShader, defaultFragmentShader);
}

core::YCustomRenderQuad::YCustomRenderQuad(const std::string &fragmentShaderText) : YRenderQuad() {
    generateProgram(defaultVertexShader, fragmentShaderText);
}

core::YCustomRenderQuad::YCustomRenderQuad(const std::string &vertexShaderText, const std::string &fragmentShaderText) : YRenderQuad() {
    generateProgram(vertexShaderText, fragmentShaderText);
}

core::YCustomRenderQuad::~YCustomRenderQuad() {
    glDeleteProgram(programme);
}

void core::YCustomRenderQuad::render() const {
    glUseProgram(programme);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(static_cast<GLint>(textureUniform), 0);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void core::YCustomRenderQuad::generateProgram(const std::string &vertexShaderText, const std::string &fragmentShaderText) {
    vertexShader = YShader::generateShader(vertexShaderText, GL_VERTEX_SHADER);
    fragmentShader = YShader::generateShader(fragmentShaderText, GL_FRAGMENT_SHADER);
    programme = YShader::generateProgram(vertexShader, fragmentShader);
    textureUniform = glGetUniformLocation(programme, "textureUniform");
}

GLuint core::YCustomRenderQuad::getProgramme() const {
    return this->programme;
}
