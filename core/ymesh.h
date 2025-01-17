//
// Created by Yvens Serpa on 07/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "ytexture.h"
#include "yvertex.h"
#include "../math/yboundingbox.h"

namespace core {
    class YMesh {
    public:
        std::vector<YVertex> vertices;
        std::vector<GLuint> indices;
        std::vector<YTexture> textures;

    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;

    public:
        YMesh(const std::vector<YVertex> &vertices, const std::vector<GLuint> &indices,
              const std::vector<YTexture> &textures);

        YMesh(const YMesh &anotherMesh);

        virtual ~YMesh() = default;

        virtual void draw(GLuint shader, GLenum mode, bool renderWithTextures) const;

        void drawPatches(GLuint shader, bool renderWithTextures) const;

        [[nodiscard]] math::YBoundingBox getBoundingBox() const;

        [[nodiscard]] const std::vector<YVertex> &getVertices() const;

        [[nodiscard]] const std::vector<GLuint> &getIndices() const;

        [[nodiscard]] const std::vector<YTexture> &getTextures() const;

        [[nodiscard]] GLuint getVAO() const;

        [[nodiscard]] GLuint getVBO() const;

        [[nodiscard]] GLuint getEBO() const;

        YMesh &operator=(const YMesh &anotherMesh);

    private:
        void setupMesh();

    protected:
        YMesh(const YMesh &anotherMesh, bool generateOwnObjects);

        void bindTextures(GLuint shader) const;

        void setVAO(GLuint VAO);

        void setVBO(GLuint VBO);

        void setEBO(GLuint EBO);

    public:
        static YVertex generateVertex(glm::vec3 position, glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec2 texCoords = glm::vec2(0.0f, 0.0f));
    };
}
