//
// Created by Yvens Serpa on 01/01/2025.
//

#include "YObjectUI.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "../elements/yobject.h"

namespace view {
    YObjectUI::YObjectUI(std::shared_ptr<elements::YObject> object) : selfObject(std::move(object)) { }

    void YObjectUI::render() const {
        ImGui::SeparatorText(selfObject->getIdentifier().c_str());
        const auto transform = selfObject->getTransform();
        auto position = transform.getPosition();
        ImGui::InputFloat4("Pos", glm::value_ptr(position));
    }

    std::string YObjectUI::getIdentifier() const {
        return selfObject->getIdentifier();
    }
} // view