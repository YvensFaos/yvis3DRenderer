//
// Created by Yvens Serpa on 16/01/2025.
//

#pragma once
#include <memory>
#include <vector>

namespace core {
    class YMaterialRenderQuad;
    class YMaterial;
}

namespace effects {

class YPostProcess {
    std::vector<std::shared_ptr<core::YMaterialRenderQuad>> renderQuads;

    public:
        explicit YPostProcess(const std::vector<std::shared_ptr<core::YMaterial>>& materials);

    virtual void render();
};

} // effects

