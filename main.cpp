#include <glm/ext/matrix_clip_space.hpp>

#include "core/yrenderer.h"
#include "elements/yskybox.h"
#include "LuaHandler/luahandler.hpp"
#include "utils/yluahelper.h"

int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YRenderer renderer(width, height, "YRenderer");
    core::YCamera& camera = renderer.getCamera();
    LuaHandler luaHandler;
    luaHandler.openFile("data/scenes/scene_simple.lua");
    utils::YLuaHelper::setupCameraPosition("cameraPosition", camera, luaHandler);
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float) width / (float) height, 0.1f, 1000.0f);

    glm::mat4 view = camera.getView();
    glm::mat4 viewProjectionMatrix = projection * view;

    elements::YSkybox skybox(std::vector<std::string>{
            "data/skyboxes/desertsky_ft.png",
            "data/skyboxes/desertsky_bc.png",
            "data/skyboxes/desertsky_up.png",
            "data/skyboxes/desertsky_dn.png",
            "data/skyboxes/desertsky_rt.png",
            "data/skyboxes/desertsky_lf.png"
        });

    do {
        renderer.startFrame();
        skybox.render(viewProjectionMatrix);
        renderer.finishFrame();
    } while (renderer.isRunning());
}
