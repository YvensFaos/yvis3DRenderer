#include "core/yrenderer.h"
#include "LuaHandler/luahandler.hpp"

//Scenes
//#include "scenes/yedgedetectionscene.h"
// #include "scenes/yfogscene.h"
// #include "scenes/yskyboxscene.h"

int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YRenderer renderer(width, height, "YRenderer");

    // scenes::YSkyBoxScene skyBoxScene(renderer, "data/scenes/skybox_scene.lua", "skybox", width, height);
    // scenes::YFogScene fogScene(renderer, "data/scenes/fog_scene.lua", width, height);
    //scenes::YEdgeDetectionScene edgeScene(renderer, "data/scenes/edge_detection_scene.lua", width, height);
    do {

    } while (renderer.isRunning());
}
