//
// Created by Yvens Serpa on 11/01/2025.
//

#include "yrotatingbehavior.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "../core/yapplication.h"
#include "../core/ytransform.h"

behaviors::YRotatingBehavior::YRotatingBehavior(core::YTransform &transform, const glm::vec3 axis,
                                                const float speed) : YGenericBehavior(), transform(transform),
                                                                     rotatingAxis(axis),
                                                                     rotationSpeed(speed) {
}

void behaviors::YRotatingBehavior::update() {
    const auto deltaTime = core::YApplication::getDeltaTime();
    transform.rotate(rotationSpeed * rotatingAxis * deltaTime);
}

void behaviors::YRotatingBehavior::toUI() {
    YGenericBehavior::toUI();
    auto axis = rotatingAxis;
    if(ImGui::InputFloat3("Pos", glm::value_ptr(axis))) {
        rotatingAxis = axis;
    }
    auto speed = rotationSpeed;
    if(ImGui::InputFloat("Speed", &speed)) {
        rotationSpeed = speed;
    }
}
