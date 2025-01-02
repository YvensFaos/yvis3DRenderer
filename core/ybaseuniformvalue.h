//
// Created by Yvens Serpa on 28/12/2024.
//

#pragma once

namespace core {
    struct YUniform;

    struct YBaseUniformValue {
    protected:
        const std::shared_ptr<core::YUniform> uniform;

    public:
        explicit YBaseUniformValue(const std::shared_ptr<core::YUniform> &uniform);

        virtual ~YBaseUniformValue() = default;

        virtual void stream() const = 0;
        virtual void updateValue(const YBaseUniformValue & another) = 0;

        [[nodiscard]] std::string getUniformName() const;
        bool operator==(const std::string& uniformName) const;
        bool operator!=(const std::string& uniformName) const;
    };
}