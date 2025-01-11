//
// Created by Yvens Serpa on 11/01/2025.
//

#include "ygeneratebehavior.h"

#include "yrotatingbehavior.h"
#include "../core/ygenericbehavior.h"
#include "../elements/yrenderobject.h"
#include "../utils/yluahelper.h"

namespace behaviors {
    std::shared_ptr<core::YGenericBehavior> YGenerateBehavior::GenerateFromLuaTable(
        const LuaHandler &handler, elements::YRenderObject &renderObject) {
        const auto name = handler.getStringFromTable("name");

        if (name == "YRotatingBehavior") {
            const auto rotatingAxis = utils::YLuaHelper::readVec3FromTableInTable("rotatingAxis", handler);
            const auto rotationSpeed = handler.getNumberFromTable("rotationSpeed");
            const auto rotatingBehavior = std::make_shared<behaviors::YRotatingBehavior>(
                renderObject.getTransform(), rotatingAxis,
                rotationSpeed);
            handler.popTable();
            return rotatingBehavior;
        }

        printf("Error - Behavior name not found: %s.\n", name.c_str());
        handler.popTable();
        return std::make_shared<core::YGenericBehavior>();
    }
} // behaviors
