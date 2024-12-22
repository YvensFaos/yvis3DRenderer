//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include "yloadedscene.h"

namespace scenes {
    YLoadedScene::YLoadedScene(core::YRenderer &renderer, const std::string &file, const int width,
                               const int height) : YScene(renderer, file, width, height) {
//        renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));
//        core::YCamera &camera = renderer.getCamera();
//        utils::YLuaHelper::setupCameraPosition("cameraPosition", camera, luaHandler);
//
//        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float) width / (float) height, 0.1f,
//                                                1000.0f);
//        glm::mat4 view = camera.getView();
//        viewProjectionMatrix = projection * view;
    }

    void YLoadedScene::renderImpl() {
        for (elements::YObject *object: objects) {
            object->draw();
            object->update();
        }
    }

    YLoadedScene::~YLoadedScene() {
        for (elements::YObject *object: objects)
            delete object;
    }
} // scenes