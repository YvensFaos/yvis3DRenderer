//
// Created by Yvens Serpa on 16/01/2025.
//

#include "yrenderquad.h"

#include "ytexture.h"

namespace core {
    YRenderQuad::YRenderQuad() : texture(nullptr) {
        initialize();
    }

    void YRenderQuad::setTexture(const std::shared_ptr<core::YTexture> &texture) {
        this->texture = texture;
    }

    void YRenderQuad::initialize() {
        static float quadVertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 0.0f, 1.0f, 0.0f,
        };

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
} // core