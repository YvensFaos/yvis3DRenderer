//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include "ymaterial.h"
#include "yuniformvalue.h"
#include "../LuaHandler/luahandler.hpp"

namespace core {

    class YMaterialInstance {
    private:
        const core::YMaterial& material;
        std::vector<YBaseUniformValue*> uniformValues;
    public:
        explicit YMaterialInstance(const core::YMaterial& material, LuaHandler& luaHandler);
    };

} // core

