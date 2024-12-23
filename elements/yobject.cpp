//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "yobject.h"

#include <utility>

elements::YObject::YObject(std::string identifier) : identifier(std::move(identifier)), modelMatrix(1) {}

glm::mat4 elements::YObject::getModelMatrix() const {
    return modelMatrix;
}

void elements::YObject::draw() {}

void elements::YObject::update() {}