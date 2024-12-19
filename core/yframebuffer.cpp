//
// Created by Yvens Serpa on 18/12/2024.
//

#include "yframebuffer.h"

#include <cstdio>
#include <limits>

core::YFrameBuffer::YFrameBuffer() : width(-1), height(-1), bufferShowFlag(std::numeric_limits<GLuint>::max()) {
}

core::YFrameBuffer::YFrameBuffer(const GLfloat width, const GLfloat height, const GLint internalFormat,
                                 const GLint format, const GLint type) : width(width), height(height),
                                                                         bufferShowFlag(
                                                                             std::numeric_limits<GLuint>::max()) {
    FBO = 0;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    framebufferTexture = 0;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format,
                 type, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);
    generateRenderbuffer();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Error creating FrameBuffer!\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

core::YFrameBuffer::~YFrameBuffer() {
    glDeleteFramebuffers(1, &FBO);
}

GLuint core::YFrameBuffer::getFBO() const {
    return FBO;
}

GLuint core::YFrameBuffer::getFramebufferTexture() const {
    return framebufferTexture;
}

GLfloat core::YFrameBuffer::getWidth() const {
    return width;
}

GLfloat core::YFrameBuffer::getHeight() const {
    return height;
}

void core::YFrameBuffer::bindBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void core::YFrameBuffer::bindBuffer(const GLuint showFlag) const {
    if (showFlag & this->bufferShowFlag) {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }
}

void core::YFrameBuffer::setViewport() const {
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void core::YFrameBuffer::unbindBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void core::YFrameBuffer::generateRenderbuffer() {
    RBO = 0;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width),
                          static_cast<GLsizei>(height));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

void core::YFrameBuffer::setBufferShowFlag(const GLuint bufferShowFlag) {
    this->bufferShowFlag = bufferShowFlag;
}

void core::YFrameBuffer::changeTextureParameter(const GLint textureParameter, const GLint textureParameterValue) const {
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexParameteri(GL_TEXTURE_2D, textureParameter, textureParameterValue);
    glBindTexture(GL_TEXTURE_2D, 0);
}
