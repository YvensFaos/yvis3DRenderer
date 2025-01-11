//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include "yobject.h"

namespace core {
    class YModel;
    class YRenderer;
    class YMaterialInstance;
}

namespace elements {
    class YRenderObject : public elements::YObject {
    private:
        std::shared_ptr<core::YModel> model;
        std::shared_ptr<core::YMaterialInstance> materialInstance;
    public:
        explicit YRenderObject(const std::string &identifier, std::shared_ptr<core::YModel> model,
                               std::shared_ptr<core::YMaterialInstance> materialInstance);

        void draw(const core::YRenderer &renderer) override;
    };
}