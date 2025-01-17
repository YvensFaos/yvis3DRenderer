//
// Created by Yvens Serpa on 17/01/2025.
//

#pragma once
#include "yrenderquad.h"

namespace core {
    class YMaterial;

    class YMaterialRenderQuad final : public YRenderQuad {
        std::shared_ptr<YMaterial> material;

    public:
        explicit YMaterialRenderQuad(const std::shared_ptr<YMaterial> &material);
        ~YMaterialRenderQuad() override;

        void render() const override;

        std::shared_ptr<YMaterial> getMaterial() const;
};

} // core

