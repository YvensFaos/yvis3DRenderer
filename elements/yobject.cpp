//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "yobject.h"

#include <utility>

elements::YObject::YObject(std::string identifier) : identifier(std::move(identifier)), transform() {}

void elements::YObject::draw(const core::YRenderer& renderer) {}

void elements::YObject::update() {}