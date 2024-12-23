//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include "yuniform.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace core {
    struct YBaseUniformValue {
    protected:
        const YUniform& uniform;
    public:
        explicit YBaseUniformValue(const YUniform& uniform) : uniform(uniform) {}
        virtual ~YBaseUniformValue() = default;
        virtual void stream() const = 0;
    };

    template<typename T>
    struct YUniformValue : public YBaseUniformValue{
    private:
        T value;
        std::function<void()> streamUniformValue;
    public:
        explicit YUniformValue(const YUniform& uniform, const T& value) : YBaseUniformValue(uniform), value(value) {
            streamUniformValue = [uniform]() {
                printf("ERROR! - Uniform %s has no stream method.\n", uniform.uniformName.c_str());
            };
        }

        void set(T newValue) {
            this->value = newValue;
        }

        void stream() const override {
            streamUniformValue();
        }
    };

    template <>
    YUniformValue<int>::YUniformValue(const YUniform& uniform, const int& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform1i(uniform.uniformLocation, value);
        };
    }

    template <>
    YUniformValue<float>::YUniformValue(const YUniform& uniform, const float& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform1f(uniform.uniformLocation, value);
        };
    }

    template <>
    YUniformValue<glm::vec2>::YUniformValue(const YUniform& uniform, const glm::vec2& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform2f(uniform.uniformLocation, value.x, value.y);
        };
    }

    template <>
    YUniformValue<glm::vec3>::YUniformValue(const YUniform& uniform, const glm::vec3& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform3f(uniform.uniformLocation, value.x, value.y, value.z);
        };
    }

    template <>
    YUniformValue<glm::vec4>::YUniformValue(const YUniform& uniform, const glm::vec4& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform4f(uniform.uniformLocation, value.x, value.y, value.z, value.w);
        };
    }

    template <>
    YUniformValue<glm::mat3>::YUniformValue(const YUniform& uniform, const glm::mat3& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniformMatrix3fv(uniform.uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
        };
    }

    template <>
    YUniformValue<glm::mat4>::YUniformValue(const YUniform& uniform, const glm::mat4& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniformMatrix4fv(uniform.uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
        };
    }

    template <>
    YUniformValue<bool>::YUniformValue(const YUniform& uniform, const bool& value) : YBaseUniformValue(uniform), value(value) {
        streamUniformValue = [uniform, value]() {
            glUniform1i(uniform.uniformLocation, value);
        };
    }
}
