//
// Created by Yvens Serpa on 18/12/2024.
//

#pragma once

#include <GL/glew.h>

namespace core {
    class YFrameBuffer {
    protected:
        GLfloat width{};
        GLfloat height{};

        GLuint FBO{};
        GLuint RBO{};
        GLuint framebufferTexture{};

        GLuint bufferShowFlag;

        YFrameBuffer();

    public:
        YFrameBuffer(GLfloat width, GLfloat height, GLint internalFormat = GL_RGB, GLint format = GL_RGB,
                     GLint type = GL_UNSIGNED_BYTE);

        ~YFrameBuffer();

        [[nodiscard]] GLuint getFBO() const;

        [[nodiscard]] GLuint getFramebufferTexture() const;

        [[nodiscard]] GLfloat getWidth() const;

        [[nodiscard]] GLfloat getHeight() const;

        void bindBuffer() const;

        void bindBuffer(GLuint showFlag) const;

        void setViewport() const;

        void unbindBuffer() const;

        void setBufferShowFlag(GLuint newBufferShowFlag);

        void changeTextureParameter(GLint textureParameter, GLint textureParameterValue) const;

    protected:
        void generateRenderbuffer();
    };
}
