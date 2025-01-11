//
// Created by Yvens Serpa on 11/01/2025.
//

#pragma once
#include <string>
#include <glm/vec3.hpp>
#include "../core/ygenericbehavior.h"

namespace core {
    struct YTransform;
}

namespace behaviors {
    class YRotatingBehavior final : public core::YGenericBehavior {
        core::YTransform &transform;
        glm::vec3 rotatingAxis;
        float rotationSpeed;

    public:
        explicit YRotatingBehavior(core::YTransform &transform, glm::vec3 axis, float speed);

        ~YRotatingBehavior() override = default;

        void update() override;

        [[nodiscard]] std::string toString() const override { return "YRotatingBehavior"; }
    };
}
