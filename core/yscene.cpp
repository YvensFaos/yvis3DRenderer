//
// Created by Yvens Serpa on 09/12/2024.
//

#include "yscene.h"

namespace core {
    YScene::YScene(YRenderer &renderer, const std::string &file, const int width, const int height) : renderer(
            renderer), width(width), height(height), fileName(file) {
        if (luaHandler.openFile(file)) {
            printf("Successfully loaded %s\n", file.c_str());
            loaded = true;
        } else {
            printf("Failed to load %s\n", file.c_str());
            loaded = false;
        }
    }

    void YScene::render(const GLuint fbo) {
        renderer.startFrame(fbo);
        renderImpl();
    }

    bool YScene::isLoaded() const {
        return loaded;
    }

    std::string YScene::getFileName() const {
        return fileName;
    }
} // core
