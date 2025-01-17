//
// Created by Yvens Serpa on 18/12/2024.
//

#include "yframebuffer.h"

#include <cstdio>
#include "ytexture.h"

core::YFrameBuffer::YFrameBuffer() : width(-1), height(-1), FBO(-1), RBO(-1), texture(nullptr) {
}

core::YFrameBuffer::YFrameBuffer(const GLfloat width, const GLfloat height, const GLint internalFormat,
                                 const GLint format, const GLint type) : width(width), height(height), RBO(-1) {
    FBO = 0;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    texture = std::make_unique<YTexture>();
    texture->generateTextureId();

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format,
                 type, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    constexpr GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    generateRenderbuffer();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Error creating FrameBuffer!\n");
    } else {
        printf("FrameBuffer created!\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

core::YFrameBuffer::~YFrameBuffer() {
    glDeleteFramebuffers(1, &FBO);
}

GLuint core::YFrameBuffer::getFBO() const {
    return FBO;
}

std::shared_ptr<core::YTexture> core::YFrameBuffer::getFramebufferTexture() const {
    return texture;
}

GLuint core::YFrameBuffer::getFramebufferTextureId() const {
    return texture->id;
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

void core::YFrameBuffer::setViewport() const {
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void core::YFrameBuffer::unbindBuffer() {
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

void core::YFrameBuffer::changeTextureParameter(const GLint textureParameter, const GLint textureParameterValue) const {
    texture->changeTextureParameter(textureParameter, textureParameterValue);
}
