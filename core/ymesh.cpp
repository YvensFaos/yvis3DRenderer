//
// Created by Yvens Serpa on 07/12/2024.
//
#include "ymesh.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

// #include "../Utils/amacrohelper.hpp"


namespace core {
    YMesh::YMesh(const std::vector<YVertex> &vertices, const std::vector<unsigned int> &indices,
                 const std::vector<YTexture> &textures)
            : vertices(vertices), indices(indices), textures(textures) {
        this->setupMesh();
    }

    YMesh::YMesh(const YMesh &anotherMesh) : vertices(anotherMesh.getVertices()), indices(anotherMesh.getIndices()),
                                             textures(anotherMesh.getTextures()),
                                             VAO(anotherMesh.getVAO()), VBO(anotherMesh.getVBO()),
                                             EBO(anotherMesh.getEBO()) {
    }

    YMesh::YMesh(const YMesh &anotherMesh, [[maybe_unused]] bool generateOwnObjects)
            : vertices(anotherMesh.vertices), indices(anotherMesh.indices), textures(anotherMesh.textures) {
        this->setupMesh();
    }

    YMesh &YMesh::operator=(const YMesh &anotherMesh) {
        if (this != &anotherMesh) {
            this->VAO = anotherMesh.getVAO();
            this->VBO = anotherMesh.getVBO();
            this->EBO = anotherMesh.getEBO();
            this->vertices = anotherMesh.getVertices();
            this->indices = anotherMesh.getIndices();
            this->textures = anotherMesh.getTextures();
        }
        return *this;
    }

    void YMesh::setupMesh() {
        glGenVertexArrays(1, &VAO);
        printf("VAO Generated = %d\n", VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(YVertex) * vertices.size()), &vertices[0],
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(unsigned int) * indices.size()),
                     &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(YVertex), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(YVertex), (void *) offsetof(YVertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(YVertex), (void *) offsetof(YVertex, TexCoords));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(YVertex), (void *) offsetof(YVertex, Tangent));
        glBindVertexArray(0);
    }

    void YMesh::bindTextures(const GLuint shader) const {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int number = 0;

        for (size_t i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);

            std::string name = textures[i].type;

            if (name == "texture_diffuse") {
                number = diffuseNr++;
            } else if (name == "texture_specular") {
                number = specularNr++;
            } else if (name == "texture_normal") {
                number = normalNr++;
            }

            std::string textIdentifier(name + std::to_string(number));
            glUniform1i(glGetUniformLocation(shader, textIdentifier.c_str()), static_cast<GLint>(i));
        }
    }

    void YMesh::setVAO(GLuint VAO) {
        this->VAO = VAO;
    }

    void YMesh::setVBO(GLuint VBO) {
        this->VBO = VBO;
    }

    void YMesh::setEBO(GLuint EBO) {
        this->EBO = EBO;
    }

    void YMesh::draw(GLuint shader, GLenum mode, bool renderWithTextures) const {
        if (renderWithTextures) {
            this->bindTextures(shader);
        }
        glBindVertexArray(VAO);
        glDrawElements(mode, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void YMesh::drawPatches(GLuint shader, bool renderWithTextures) const {
        if (renderWithTextures) {
            this->bindTextures(shader);
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_PATCHES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    math::YBoundingBox YMesh::getBoundingBox() const {
        glm::vec3 min(+999999.0f, +999999.0f, +999999.0f);
        glm::vec3 max(-999999.0f, -999999.0f, -999999.0f);

        for (const auto &[Position, Normal, Tangent, TexCoords]: this->vertices) {
            glm::vec3 position = Position;
            min.x = std::min(min.x, position.x);
            min.y = std::min(min.y, position.y);
            min.z = std::min(min.z, position.z);

            max.x = std::max(max.x, position.x);
            max.y = std::max(max.y, position.y);
            max.z = std::max(max.z, position.z);
        }

        return {min, max};
    }

    const std::vector<YVertex> &YMesh::getVertices() const {
        return this->vertices;
    }

    const std::vector<GLuint> &YMesh::getIndices() const {
        return this->indices;
    }

    const std::vector<YTexture> &YMesh::getTextures() const {
        return this->textures;
    }

    GLuint YMesh::getVAO() const {
        return this->VAO;
    }

    GLuint YMesh::getVBO() const {
        return this->VBO;
    }

    GLuint YMesh::getEBO() const {
        return this->EBO;
    }

    YVertex YMesh::generateVertex(glm::vec3 position, glm::vec3 normal, glm::vec3 tangent, glm::vec2 texCoords) {
        YVertex generatedVertex{};
        generatedVertex.Position = position;
        generatedVertex.Normal = normal;
        generatedVertex.Tangent = tangent;
        generatedVertex.TexCoords = texCoords;

        return generatedVertex;
    }

    ///YInstanceMesh

    YInstanceMesh::YInstanceMesh(const std::vector<YVertex> &vertices, const std::vector<GLuint> &indices,
                                 const std::vector<YTexture> &textures,
                                 const std::vector<glm::mat4> &instanceData) : YMesh(vertices, indices, textures),
                                                                               instanceData(instanceData), IBO(0) {
        this->setupInstanceBO();
    }

    YInstanceMesh::YInstanceMesh(const YInstanceMesh &anotherInstanceMesh) : YMesh(anotherInstanceMesh),
                                                                             instanceData(
                                                                                     anotherInstanceMesh.getInstanceData()),
                                                                             IBO(anotherInstanceMesh.getIBO()) {
    }

    YInstanceMesh::YInstanceMesh(const YMesh &meshToCopyFrom,
                                 std::vector<glm::mat4> instanceData) : YMesh(meshToCopyFrom, true),
                                                                        instanceData(std::move(instanceData)), IBO(0) {
        this->setupInstanceBO();
    }

    void YInstanceMesh::draw(GLuint shader, GLenum mode, bool renderWithTextures) const {
        if (renderWithTextures) {
            this->bindTextures(shader);
        }

        glBindVertexArray(this->getVAO());
        glDrawElementsInstanced(mode, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0,
                                static_cast<GLsizei>(this->getInstanceCount()));
    }

    const std::vector<glm::mat4> &YInstanceMesh::getInstanceData() const {
        return this->instanceData;
    }

    GLuint YInstanceMesh::getInstanceCount() const {
        return this->instanceData.size();
    }

    GLuint YInstanceMesh::getIBO() const {
        return this->IBO;
    }

    void YInstanceMesh::setupInstanceBO() {
        // constexpr auto sizeOfMat4 = sizeof(glm::mat4);

        glBindVertexArray(this->getVAO());

        glGenBuffers(1, &this->IBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->IBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(sizeof(glm::mat4)) * this->getInstanceCount(),
                     &instanceData[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) 0); //(0 * sizeof(glm::vec4)
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *) (3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);

        glBindVertexArray(0);
    }

    glm::mat4 YInstanceMesh::fromValuesToInstanceMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        auto instanceMatrix = glm::mat4(1.0f);
        instanceMatrix = glm::translate(instanceMatrix, position);
        instanceMatrix = glm::scale(instanceMatrix, scale);
        instanceMatrix = glm::rotate(instanceMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        instanceMatrix = glm::rotate(instanceMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        instanceMatrix = glm::rotate(instanceMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        return instanceMatrix;
    }

    YInstanceMesh &YInstanceMesh::operator=(const core::YInstanceMesh &anotherMesh) {
        if (this != &anotherMesh) {
            this->setVAO(anotherMesh.getVAO());
            this->setVBO(anotherMesh.getVBO());
            this->setEBO(anotherMesh.getEBO());
            this->IBO = anotherMesh.getIBO();
            this->vertices = anotherMesh.getVertices();
            this->indices = anotherMesh.getIndices();
            this->textures = anotherMesh.getTextures();
            this->instanceData = anotherMesh.getInstanceData();
        }

        return *this;
    }
}
