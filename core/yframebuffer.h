//
// Created by Yvens Serpa on 18/12/2024.
//

#pragma once

#include <memory>
#include <GL/glew.h>

namespace core {
    struct YTexture;
}

namespace core {
    class YFrameBuffer {
    protected:
        GLfloat width;
        GLfloat height;
        GLuint FBO;
        GLuint RBO;
        std::shared_ptr<core::YTexture> texture;
        YFrameBuffer();
    public:
        YFrameBuffer(GLfloat width, GLfloat height, GLint internalFormat = GL_RGB, GLint format = GL_RGB,
                     GLint type = GL_UNSIGNED_BYTE);

        ~YFrameBuffer();

        [[nodiscard]] GLuint getFBO() const;

        [[nodiscard]] std::shared_ptr<YTexture> getFramebufferTexture() const;

        [[nodiscard]] GLuint getFramebufferTextureId() const;

        [[nodiscard]] GLfloat getWidth() const;

        [[nodiscard]] GLfloat getHeight() const;

        void bindBuffer() const;

        void setViewport() const;

        static void unbindBuffer() ;

        void changeTextureParameter(GLint textureParameter, GLint textureParameterValue) const;

    protected:
        void generateRenderbuffer();
    };
}
