//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//
#pragma once

#include <vector>
#include "../core/yscene.h"
#include "../elements/yobject.h"

namespace scenes {

class YLoadedScene : public core::YScene {
private:
    std::vector<elements::YObject*> objects;
public:
    explicit YLoadedScene(core::YRenderer &renderer, const std::string &file, int width, int height);

    YLoadedScene(const YLoadedScene &fogScene) = delete;

    YLoadedScene &operator=(const YLoadedScene &fogScene) = delete;

    ~YLoadedScene() override;
protected:
    void renderImpl() override;
};
} // scenes

