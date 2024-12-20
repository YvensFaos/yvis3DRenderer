//
// Created by Yvens Serpa on 09/12/2024.
//

#include "yscene.h"

namespace core {
    YScene::YScene(YRenderer &renderer, const std::string &file, const int width, const int height) : renderer(renderer), width(width), height(height) {
        if (luaHandler.openFile(file)) {
            printf("Successfully loaded %s.lua\n", file.c_str());
        } else {
            printf("Failed to load %s.lua\n", file.c_str());
        }
    }

    void YScene::render() {
        renderer.startFrame();
        renderImpl();
        renderer.finishFrame();
    }
} // core
