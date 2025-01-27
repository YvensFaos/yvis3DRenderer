//
// Created by Yvens Serpa on 16/01/2025.
//

#include "ypostprocess.h"
#include "../core/ymaterial.h"
#include "../core/ymaterialrenderquad.h"

namespace effects {
    YPostProcess::YPostProcess(const std::vector<std::shared_ptr<core::YMaterial>>& materials) {
        for (auto material : materials) {
            auto renderQuad = std::make_shared<core::YMaterialRenderQuad>(material);
            renderQuads.push_back(renderQuad);
        }
    }

    void YPostProcess::render() {
        for (const auto& renderQuad : renderQuads) {
            renderQuad->render();
        }
    }
} // effects