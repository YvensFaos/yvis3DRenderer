//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "yobject.h"

elements::YObject::YObject(std::string identifier) : identifier(std::move(identifier)) {}

void elements::YObject::draw(const core::YRenderer &renderer) {}

void elements::YObject::update() {}

std::string elements::YObject::getIdentifier() const {
    return identifier;
}

core::YTransform& elements::YObject::getTransform() {
    return transform;
}
