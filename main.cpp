#include "core/yrenderer.h"

int main() {
    core::YRenderer renderer(800, 600, "YRenderer");
    renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

    

    do {
        renderer.startFrame();
        renderer.finishFrame();
    } while (renderer.isRunning());
}
