//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#include <glm/glm.hpp>
#include <string>
#include <utility>
#include "../core/ytransform.h"

namespace core {
    class YRenderer;
}

namespace elements {
    class YObject {
    public:
        core::YTransform transform;
    private:
        const std::string identifier;

    public:
        explicit YObject(std::string identifier);

        virtual ~YObject() = default;

        virtual void draw(const core::YRenderer &renderer);

        virtual void update();
    };
}
