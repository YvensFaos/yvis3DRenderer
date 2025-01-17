//
// Created by Yvens Serpa on 16/01/2025.
//

#pragma once
#include <memory>
#include <GL/glew.h>

namespace core {
    struct YTexture;

    class YRenderQuad {
    protected:
        GLuint quadVAO;
        GLuint quadVBO;
        std::shared_ptr<core::YTexture> texture;
    public:
        YRenderQuad();
        virtual ~YRenderQuad() = default;
        virtual void render() const = 0;

        void setTexture(const std::shared_ptr<core::YTexture> &texture);
    protected:
        virtual void initialize();
    };
} // core
