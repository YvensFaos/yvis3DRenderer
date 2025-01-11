//
// Created by Yvens Serpa on 11/01/2025.
//

#include "yrotatingbehavior.h"

#include "../core/ytransform.h"

behaviors::YRotatingBehavior::YRotatingBehavior(core::YTransform &transform, const glm::vec3 axis,
                                                const float speed) : YGenericBehavior(), transform(transform),
                                                                     rotatingAxis(axis),
                                                                     rotationSpeed(speed) {
}

void behaviors::YRotatingBehavior::update() {
    transform.rotate(rotationSpeed * rotatingAxis);
}
