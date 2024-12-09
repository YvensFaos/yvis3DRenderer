//
// Created by Yvens Serpa on 09/12/2024.
//

#pragma once

#include <glm/glm.hpp>

namespace elements {
    struct YFog {
        const float maxDist;
        const float minDist;
        const glm::vec4 color;

        YFog(const float maxDist, const float minDist, const glm::vec4 color) : maxDist(maxDist), minDist(minDist),
            color(color) {
        }

        YFog(const YFog &otherFog) : maxDist(otherFog.maxDist), minDist(otherFog.minDist), color(otherFog.color) {
        }

        YFog &operator=(const YFog &otherFog) = delete;
        //     if (this != &otherFog) {
        //         this->maxDist = otherFog.maxDist;
        //         this->minDist = otherFog.minDist;
        //         this->color = otherFog.color;
        //     }
        //     return *this;
        // }
    };
}
