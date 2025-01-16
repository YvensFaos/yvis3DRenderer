//
// Created by Yvens Serpa on 16/01/2025.
//

#pragma once
#include <memory>
#include <vector>

namespace core {
    class YRenderQuad;
}

namespace core {
    struct YBaseUniformValue;
    class YMaterial;
}

namespace effects {

class YPostProcess {
    std::shared_ptr<core::YMaterial> material;
    std::shared_ptr<core::YRenderQuad> renderQuad;
    std::vector<std::shared_ptr<core::YBaseUniformValue> > uniformValues;

    public:
        explicit YPostProcess(float width, float height, const std::shared_ptr<core::YMaterial> &material);
};

} // effects

