//
// Created by Yvens Serpa on 07/12/2024.
//

#include "yboundingbox.h"

namespace math {
    YBoundingBox::YBoundingBox() : min(0.0f, 0.0f, 0.0f), max(0.0f, 0.0f, 0.0f) {
    }

    YBoundingBox::YBoundingBox(const glm::vec3 min, const glm::vec3 max) : min(min), max(max) {
    }

    YBoundingBox::YBoundingBox(const YBoundingBox &anotherBoundingBox) : min(anotherBoundingBox.getMin()),
                                                                         max(anotherBoundingBox.getMax()) {
    }

    YBoundingBox &YBoundingBox::operator=(const YBoundingBox &anotherBoundingBox) {
        if (this != &anotherBoundingBox) {
            this->min = anotherBoundingBox.getMin();
            this->max = anotherBoundingBox.getMax();
        }
        return *this;
    }

    glm::vec3 YBoundingBox::getMin() const {
        return this->min;
    }

    glm::vec3 YBoundingBox::getMax() const {
        return this->max;
    }
} // math
