//
// Created by Yvens Serpa on 28/12/2024.
//

#include "yuniform.h"
#include "ybaseuniformvalue.h"

core::YBaseUniformValue::YBaseUniformValue(const std::shared_ptr<core::YUniform> &uniform) : uniform(uniform) { }

bool core::YBaseUniformValue::operator==(const std::string &uniformName) const {
    return uniform->uniformName == uniformName;
}

bool core::YBaseUniformValue::operator!=(const std::string &uniformName) const {
    return uniform->uniformName != uniformName;
}
