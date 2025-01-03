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
    YUniformValue<int>::YUniformValue(std::shared_ptr<YUniform> uniform, const int &value) : YBaseUniformValue(uniform),
                                                                                                    value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform1i(uniform->uniformLocation, value);
        };
    }

    template<>
    YUniformValue<float>::YUniformValue(std::shared_ptr<YUniform> uniform, const float &value) : YBaseUniformValue(
            uniform),
        value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform1f(uniform->uniformLocation, value);
        };
    }

    template<>
    YUniformValue<glm::vec2>::YUniformValue(std::shared_ptr<YUniform> uniform, const glm::vec2 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform2f(uniform->uniformLocation, value.x, value.y);
        };
    }

    template<>
    YUniformValue<glm::vec3>::YUniformValue(std::shared_ptr<YUniform> uniform, const glm::vec3 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform3f(uniform->uniformLocation, value.x, value.y, value.z);
        };
    }

    template<>
    YUniformValue<glm::vec4>::YUniformValue(std::shared_ptr<YUniform> uniform, const glm::vec4 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform4f(uniform->uniformLocation, value.x, value.y, value.z, value.w);
        };
    }

    template<>
    YUniformValue<glm::mat3>::YUniformValue(std::shared_ptr<YUniform> uniform, const glm::mat3 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniformMatrix3fv(uniform->uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
        };
    }

    template<>
    YUniformValue<glm::mat4>::YUniformValue(std::shared_ptr<YUniform> uniform, const glm::mat4 &value)
        : YBaseUniformValue(
              uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniformMatrix4fv(uniform->uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
        };
    }

    template<>
    YUniformValue<bool>::YUniformValue(std::shared_ptr<YUniform> uniform, const bool &value) : YBaseUniformValue(
            uniform),
        value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform1i(uniform->uniformLocation, value);
        };
    }
}