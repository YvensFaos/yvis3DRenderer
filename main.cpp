#include "core/yapplication.h"
#include <string>

int main() {
    constexpr int width = 1000;
    constexpr int height = 900;
    core::YApplication application(width, height, "YApplication");
    application.run();
}
