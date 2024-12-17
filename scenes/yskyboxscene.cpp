//
// Created by Yvens Serpa on 17/12/2024.
//

#include "yskyboxscene.h"
#include "yfogscene.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "../core/yshader.h"
#include "../utils/yluahelper.h"

namespace scenes {
    YSkyBoxScene::YSkyBoxScene(core::YRenderer &renderer, const std::string &file, const std::string &skyboxTable, const int width,
                               const int height) : YScene(renderer, file, width, height),
                                                   skybox(utils::YLuaHelper::readListOfStringsFromTable
                                                       (skyboxTable, luaHandler)) {
        renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

        core::YCamera &camera = renderer.getCamera();
        utils::YLuaHelper::setupCameraPosition("cameraPosition", camera, luaHandler);

        glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float) width / (float) height, 0.1f,
                                                1000.0f);
        glm::mat4 view = camera.getView();
        viewProjectionMatrix = projection * view;
    }

    void YSkyBoxScene::renderImpl() {
        skybox.render(viewProjectionMatrix);
    }
} // scenes
