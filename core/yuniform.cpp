//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#include "yuniform.h"

namespace core {
    YUniform::YUniform(std::string uniformName, const YUniformType &type) : uniformName(std::move(uniformName)), type(type) { }

    YUniform& YUniform::operator=(const YUniform &another) {
        if(this != &another) {
            uniformName = another.uniformName;
            type = another.type;
        }
        return *this;
    }

    bool YUniform::operator<(const YUniform &anotherUniform) const {
        return uniformName < anotherUniform.uniformName;
    }
}