//
// Created by Yvens Serpa on 03/01/2025.
//

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "yuniformvalue.h"

namespace core {
    template<>
    YUniformValue<int>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const int &value) : YBaseUniformValue(uniform),
        value(value) {
        streamUniformValue = [&]() {
            glUniform1i(uniform->uniformLocation, this->value);
        };
    }

    template<>
    YUniformValue<float>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const float &value) : YBaseUniformValue(
            uniform),
        value(value) {
        streamUniformValue = [&]() {
            glUniform1f(uniform->uniformLocation, this->value);
        };
    }

    template<>
    YUniformValue<glm::vec2>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const glm::vec2 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [&]() {
            glUniform2f(uniform->uniformLocation, this->value.x, this->value.y);
        };
    }

    template<>
    YUniformValue<glm::vec3>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const glm::vec3 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [&]() {
            glUniform3f(uniform->uniformLocation, this->value.x, this->value.y, this->value.z);
        };
    }

    template<>
    YUniformValue<glm::vec4>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const glm::vec4 &value)
        : YBaseUniformValue(
              uniform), value(value) {

        streamUniformValue = [&]() {
//            printf("Stream uniform %s %d %f %f %f %f.\n", uniform->getUniformName().c_str(), uniform->uniformLocation, this->value.x, this->value.y, this->value.z, this->value.w);
            glUniform4f(uniform->uniformLocation, this->value.x, this->value.y, this->value.z, this->value.w);
        };
    }

    template<>
    YUniformValue<glm::mat3>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const glm::mat3 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [&]() {
            glUniformMatrix3fv(uniform->uniformLocation, 1, GL_FALSE, glm::value_ptr(this->value));
        };
    }

    template<>
    YUniformValue<glm::mat4>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const glm::mat4 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [&]() {
            glUniformMatrix4fv(uniform->uniformLocation, 1, GL_FALSE, glm::value_ptr(this->value));
        };
    }

    template<>
    YUniformValue<bool>::YUniformValue(const std::shared_ptr<YUniform> &uniform, const bool &value) : YBaseUniformValue(
            uniform),
        value(value) {
        streamUniformValue = [&]() {
            glUniform1i(uniform->uniformLocation, this->value);
        };
    }
}
