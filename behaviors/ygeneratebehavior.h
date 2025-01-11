//
// Created by Yvens Serpa on 11/01/2025.
//

#pragma once

#include "../LuaHandler/luahandler.hpp"

namespace elements {
    class YRenderObject;
}

namespace core {
    class YGenericBehavior;
}

namespace behaviors {
    class YGenerateBehavior {
    public:
        static std::shared_ptr<core::YGenericBehavior> GenerateFromLuaTable(
            const LuaHandler &handler, elements::YRenderObject &renderObject);
    };
} // behaviors
