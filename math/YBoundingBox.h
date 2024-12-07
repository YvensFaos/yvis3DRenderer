//
// Created by Yvens Serpa on 07/12/2024.
//

#pragma once

#include <glm/glm.hpp>

namespace math {
    class YBoundingBox {
        glm::vec3 min;
        glm::vec3 max;

    public:
        YBoundingBox();

        YBoundingBox(glm::vec3 min, glm::vec3 max);

        YBoundingBox(const YBoundingBox &anotherBoundingBox);

        ~YBoundingBox() = default;

        YBoundingBox &operator=(const YBoundingBox &anotherBoundingBox);

        [[nodiscard]] glm::vec3 getMin() const;

        [[nodiscard]] glm::vec3 getMax() const;
    };
} // math
