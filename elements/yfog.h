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

        explicit operator bool() const;
        [[nodiscard]] std::string toString() const;
    };

    inline YFog::operator bool() const {
        return maxDist != -1 && minDist != -1;
    }

    inline std::string YFog::toString() const {
        static char buffer[196];
        snprintf(buffer, 196, "Fog[%4.2f,%4.2f,C[%4.2f,%4.2f,%4.2f,%4.2f]]", minDist, maxDist, color.r, color.g, color.b, color.a);
        return std::string(buffer);
    }
}
