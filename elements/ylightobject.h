//
// Created by Yvens Serpa on 02/01/2025.
//

#pragma once
#include "yobject.h"

namespace core {
    class YMaterialInstance;
}

namespace core {
    class YModel;
    class YMaterial;
}

namespace core {
    class YLight;
}

namespace elements {

class YLightObject final : public YObject {
    std::shared_ptr<core::YLight> light;
    std::shared_ptr<core::YModel> lightModel;
    std::shared_ptr<core::YMaterialInstance> materialInstance;

    static const std::string defaultLightModelPath;
    static const std::string defaultVertexShader;
    static const std::string defaultGeometryShader;
    static const std::string defaultFragmentShader;
public:
    explicit YLightObject(const std::string &identifier, const std::shared_ptr<core::YLight> &light);

    void draw(const core::YRenderer &renderer) override;

    static std::shared_ptr<core::YMaterial> getDefaultMaterial();
};

} // elements

