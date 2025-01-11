//
// Created by Yvens Serpa on 11/01/2025.
//

#pragma once
#include <string>

namespace core {
    class YGenericBehavior {
    public:
        YGenericBehavior() = default;

        virtual ~YGenericBehavior() = default;

        YGenericBehavior(const YGenericBehavior &) = delete;

        YGenericBehavior &operator=(const YGenericBehavior &) = delete;

        virtual void update() {};

        virtual std::string toString() const { return "YGenericBehavior"; }
    };
}
