//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#pragma once

#include <string>
#include <vector>
#include "../core/ytransform.h"

namespace core {
    class YGenericBehavior;
    class YRenderer;
}

namespace elements {
    class YObject {
    public:
        core::YTransform transform;

    private:
        const std::string identifier;
        std::vector<std::shared_ptr<core::YGenericBehavior> > behaviors;

    public:
        explicit YObject(std::string identifier);

        virtual ~YObject() = default;

        virtual void draw(const core::YRenderer &renderer);

        virtual void update();

        [[nodiscard]] std::string getIdentifier() const;

        core::YTransform &getTransform();

        void addBehavior(std::shared_ptr<core::YGenericBehavior> behavior);

        [[nodiscard]] int getBehaviorCount() const;

        std::vector<std::shared_ptr<core::YGenericBehavior> >::iterator getBehaviorIterator();

        std::vector<std::shared_ptr<core::YGenericBehavior> >::iterator getBehaviorEndIterator();
    };
}
