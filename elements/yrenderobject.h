//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include "yobject.h"

namespace core {
    class YModel;

    class YMaterial;
}

namespace elements {
    class YRenderObject : public elements::YObject {

    private:
        std::shared_ptr<core::YModel> model;
        std::shared_ptr<core::YMaterial> material;
    public:
        explicit YRenderObject(const std::string &identifier);

        void draw() override;
    };
}


