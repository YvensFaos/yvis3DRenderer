//
// Created by Yvens Serpa on 11/01/2025.
//

#pragma once

namespace core {
    class YGenericBehavior {
    public:
        YGenericBehavior() = default;

        virtual ~YGenericBehavior() = default;

        YGenericBehavior(const YGenericBehavior &) = delete;

        YGenericBehavior &operator=(const YGenericBehavior &) = delete;

        virtual void update() {};
    };
}
