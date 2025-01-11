//
// Created by Yvens Serpa on 01/01/2025.
//

#pragma once
#include <memory>
#include <glm/vec4.hpp>

namespace elements {
    class YObject;
}

namespace view {
    class YObjectUI {
        std::shared_ptr<elements::YObject> selfObject;
        glm::vec4 objectPosition;

    public:
        explicit YObjectUI(std::shared_ptr<elements::YObject> object);

        ~YObjectUI() = default;

        YObjectUI(const YObjectUI &) = default;

        void render();

        std::string getIdentifier() const;
    };
} // view
