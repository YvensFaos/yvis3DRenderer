//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "yobject.h"

#include "../core/ygenericbehavior.h"

elements::YObject::YObject(std::string identifier) : identifier(std::move(identifier)) {}

void elements::YObject::draw(const core::YRenderer &renderer) {}

void elements::YObject::update() {
    for (const auto& behavior: behaviors) {
        behavior->update();
    }
}

std::string elements::YObject::getIdentifier() const {
    return identifier;
}

core::YTransform& elements::YObject::getTransform() {
    return transform;
}

void elements::YObject::addBehavior(std::shared_ptr<core::YGenericBehavior> behavior) {
    behaviors.emplace_back(std::move(behavior));
}
