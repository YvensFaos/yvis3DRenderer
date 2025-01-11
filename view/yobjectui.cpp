//
// Created by Yvens Serpa on 01/01/2025.
//

#include "yobjectui.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "../core/ygenericbehavior.h"
#include "../elements/yobject.h"

namespace view {
    YObjectUI::YObjectUI(std::shared_ptr<elements::YObject> object) : selfObject(std::move(object)),
                                                                      objectPosition(
                                                                          selfObject->getTransform().getPosition()) {
    }

    void YObjectUI::render() {
        ImGui::SeparatorText(selfObject->getIdentifier().c_str());
        if (ImGui::InputFloat4("Pos", glm::value_ptr(objectPosition))) {
            selfObject->getTransform().setPosition(objectPosition);
        }
        ImGui::Text("Behaviors: %d", selfObject->getBehaviorCount());
        auto begin = selfObject->getBehaviorIterator();
        while(begin != selfObject->getBehaviorEndIterator()) {
            ImGui::Text("%s", (*begin)->toString().c_str());
            ++begin;
        }
    }

    std::string YObjectUI::getIdentifier() const {
        return selfObject->getIdentifier();
    }
} // view
