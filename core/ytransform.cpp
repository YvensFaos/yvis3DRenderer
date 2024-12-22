//
// Created by Yvens Rebouças Serpa on 25/12/2024.
//

#include "ytransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace core {

    void YTransform::translate(glm::vec3 translateTo) {
        this->modelMatrix = glm::translate(this->modelMatrix, translateTo);
    }

    void YTransform::rotate(glm::vec3 rotateTo) {
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotateTo.x), glm::vec3(1, 0, 0));
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotateTo.y), glm::vec3(0, 1, 0));
        this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotateTo.z), glm::vec3(0, 0, 1));
    }

    void YTransform::scale(glm::vec3 scaleTo) {
        this->modelMatrix = glm::scale(this->modelMatrix, scaleTo);
    }

    glm::mat4 YTransform::getModelMatrix() const {
        return this->modelMatrix;
    }

    glm::vec4 YTransform::getPosition() const {
        return this->modelMatrix[3];
    }

    void YTransform::setPosition(glm::vec3 position) {
        this->modelMatrix[3] = glm::vec4(position, 1.0f);
    }

    void YTransform::setModelMatrix(glm::mat4 matrix) {
        this->modelMatrix = matrix;
    }
} // core