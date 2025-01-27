//
// Created by Yvens Serpa on 17/01/2025.
//

#include "ymaterialrenderquad.h"

namespace core {
    YMaterialRenderQuad::YMaterialRenderQuad(const std::shared_ptr<YMaterial> &material) : material(material) {

    }

    YMaterialRenderQuad::~YMaterialRenderQuad() {
    }

    void YMaterialRenderQuad::render() const {
    }

    std::shared_ptr<YMaterial> YMaterialRenderQuad::getMaterial() const {
        return material;
    }
} // core