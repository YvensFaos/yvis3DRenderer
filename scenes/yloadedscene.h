//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#pragma once

#include <vector>
#include "../core/yscene.h"
#include "../core/ymaterial.h"
#include "../elements/yrenderobject.h"

namespace scenes {

    class YLoadedScene : public core::YScene {
    private:
        std::vector<std::shared_ptr<elements::YObject>> objects;
        std::unordered_map<std::string, std::shared_ptr<core::YMaterial>> materials;
        std::unordered_map<std::string, std::shared_ptr<core::YModel>> models;
    public:
        explicit YLoadedScene(core::YRenderer &renderer, const std::string &file, int width, int height);

        YLoadedScene(const YLoadedScene &fogScene) = delete;

        YLoadedScene &operator=(const YLoadedScene &fogScene) = delete;

        ~YLoadedScene() override = default;

    protected:
        void renderImpl() override;
    };
} // scenes

