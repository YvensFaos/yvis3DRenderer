#include "core/yapplication.h"
#include <string>

int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YApplication application(width, height, "YApplication");
    application.run();
}
