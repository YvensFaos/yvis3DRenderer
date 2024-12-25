//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#pragma once

#include <string>
#include <utility>

namespace core {
    enum YUniformType {
        INT, FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4, BOOL, SAMPLER2D, CUSTOM
    };

    struct YUniform {
        std::string uniformName;
        core::YUniformType type;
        int uniformLocation{};
    public:

        YUniform(std::string uniformName, const core::YUniformType &type);

        YUniform(const YUniform &uniform) = default;

        YUniform &operator=(const YUniform &another);

        bool operator<(const YUniform &anotherUniform) const;

        [[nodiscard]] std::string getUniformName() const;

        [[nodiscard]] core::YUniformType getType() const;

        int getUniformLocation() const;

        void setUniformName(std::string newUniformName);

        void setUniformType(core::YUniformType newType);

        void setUniformLocation(int newLocation);
    };
}