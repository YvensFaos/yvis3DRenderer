//
// Created by Yvens Serpa on 16/01/2025.
//

#include "yinstancemesh.h"

#include <glm/ext/matrix_transform.hpp>

namespace core {
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
} // core
