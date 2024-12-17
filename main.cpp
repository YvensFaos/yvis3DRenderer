#include "core/yrenderer.h"
#include "LuaHandler/luahandler.hpp"
#include "scenes/yskyboxscene.h"
int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YRenderer renderer(width, height, "YRenderer");

    scenes::YSkyBoxScene skyBoxScene(renderer, "data/scenes/skybox_scene.lua", "skybox", 800, 800);
    do {
        skyBoxScene.render();
    } while (renderer.isRunning());
}
