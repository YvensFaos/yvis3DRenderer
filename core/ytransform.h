//
// Created by Yvens Rebouças Serpa on 25/12/2024.
//

#pragma once

#include <glm/glm.hpp>

namespace core {

    struct YTransform {
        glm::mat4 modelMatrix{};

    public:
        YTransform() : modelMatrix(1.0f) {}

        explicit YTransform(glm::mat4 matrix) : modelMatrix(matrix) {}

        ~YTransform() = default;

        void translate(glm::vec3 translateTo);

        void rotate(glm::vec3 rotateTo);

        void scale(glm::vec3 scaleTo);

        [[nodiscard]] glm::mat4 getModelMatrix() const;

        [[nodiscard]] glm::vec4 getPosition() const;

        void setModelMatrix(const glm::mat4 &);

        void setPosition(glm::vec3 position);
    };

}
