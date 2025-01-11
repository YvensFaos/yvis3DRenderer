//
// Created by Yvens Rebouças Serpa on 25/12/2024.
//

#include "ytransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace core {

    void YTransform::translate(const glm::vec3 translateTo) {
        this->modelMatrix = glm::translate(this->modelMatrix, translateTo);
    }

    void YTransform::rotate(const glm::vec3 rotateTo) {
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotateTo.x), glm::vec3(1, 0, 0));
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotateTo.y), glm::vec3(0, 1, 0));
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotateTo.z), glm::vec3(0, 0, 1));
    }

    void YTransform::scale(const glm::vec3 scaleTo) {
        this->modelMatrix = glm::scale(this->modelMatrix, scaleTo);
    }

    glm::mat4 YTransform::getModelMatrix() const {
        return this->modelMatrix;
    }

    glm::vec4 YTransform::getPosition() const {
        return this->modelMatrix[3];
    }

    void YTransform::setPosition(const glm::vec3 position) {
        this->modelMatrix[3] = glm::vec4(position, 1.0f);
    }

    void YTransform::setModelMatrix(const glm::mat4 &matrix) {
        this->modelMatrix = matrix;
    }
} // core