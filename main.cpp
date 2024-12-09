#include "core/yrenderer.h"
#include "scenes/yfogscene.h"

int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YRenderer renderer(width, height, "YRenderer");

    scenes::YFogScene fogScene(renderer, "data/scenes/scene_simple.lua", width, height);

    do {
        fogScene.render();
    } while (renderer.isRunning());
}
