//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include "yuniform.h"
#include "ybaseuniformvalue.h"

namespace core {
    template<typename T>
    struct YUniformValue final : YBaseUniformValue {
    private:
        T value;
        std::function<void()> streamUniformValue;

    public:
        explicit YUniformValue(std::shared_ptr<core::YUniform> uniform, const T &value) : YBaseUniformValue(uniform),
            value(value) {
            streamUniformValue = [uniform]() {
                printf("ERROR! - Uniform %s has no stream method.\n", uniform->uniformName.c_str());
            };
        }

        explicit YUniformValue(const std::shared_ptr<core::YUniform> &uniform, const T &value,
                               const std::function<void()>& streamFunction) : YBaseUniformValue(uniform),
                                                                       value(value) {
            streamUniformValue = [streamFunction]() {
                streamFunction();
            };
        }

        void set(T newValue) {
            this->value = newValue;
        }

        T get() const {
            return value;
        }

        void updateValue(const YBaseUniformValue & another) override {
            const YUniformValue* derived = dynamic_cast<const YUniformValue*>(&another);
            if (derived) {
                set(derived->get());
            }
        }

        void stream() const override {
            streamUniformValue();
        }
    };
}
