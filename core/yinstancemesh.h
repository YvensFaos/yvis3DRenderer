//
// Created by Yvens Serpa on 16/01/2025.
//

#pragma once
#include <vector>
#include <glm/fwd.hpp>

#include "ymesh.h"

namespace core {
    class YInstanceMesh final : public YMesh {
    public:
        std::vector<glm::mat4> instanceData;

    private:
        GLuint IBO;

    public:
        YInstanceMesh(const std::vector<YVertex> &vertices, const std::vector<GLuint> &indices,
                      const std::vector<YTexture> &textures,
                      const std::vector<glm::mat4> &instanceData);

        YInstanceMesh(const YInstanceMesh &anotherInstanceMesh);

        YInstanceMesh(const YMesh &meshToCopyFrom, std::vector<glm::mat4> instanceData);

        [[nodiscard]] const std::vector<glm::mat4> &getInstanceData() const;

        [[nodiscard]] GLuint getInstanceCount() const;

        [[nodiscard]] GLuint getIBO() const;

        void draw(GLuint shader, GLenum mode, bool renderWithTextures) const override;

        YInstanceMesh &operator=(const YInstanceMesh &anotherMesh);

    private:
        void setupInstanceBO();

    public:
        static glm::mat4 fromValuesToInstanceMatrix(glm::vec3 position,
                                                    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                                                    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
    };
} // core
