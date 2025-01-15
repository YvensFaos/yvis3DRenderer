//
// Created by Yvens Serpa on 15/01/2025.
//

#include "ygenericbehavior.h"

#include <imgui.h>

namespace core {
    void YGenericBehavior::toUI() {
        ImGui::Text("%s", toString().c_str());
    }
}
