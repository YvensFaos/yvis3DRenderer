//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#include "yuniform.h"

#include <utility>

namespace core {
    YUniform::YUniform(std::string uniformName, const YUniformType &type) :
            uniformName(std::move(uniformName)),
            type(type),
            uniformLocation(-1) {}

    YUniform &YUniform::operator=(const YUniform &another) {
        if (this != &another) {
            uniformName = another.uniformName;
            type = another.type;
            uniformLocation = another.uniformLocation;
        }
        return *this;
    }

    bool YUniform::operator<(const YUniform &anotherUniform) const {
        return uniformName < anotherUniform.uniformName;
    }
}

std::string core::YUniform::getUniformName() const {
    return uniformName;
}

core::YUniformType core::YUniform::getType() const {
    return type;
}

int core::YUniform::getUniformLocation() const {
    return uniformLocation;
}

void core::YUniform::setUniformName(std::string newUniformName) {
    uniformName = std::move(newUniformName);
}

void core::YUniform::setUniformType(core::YUniformType newType) {
    type = newType;
}

void core::YUniform::setUniformLocation(int newLocation) {
    uniformLocation = newLocation;
}

