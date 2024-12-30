//
// Created by Yvens Serpa on 28/12/2024.
//

#pragma once
#include <string>

namespace utils {
    class YGLHelper {
    public:
        static void checkGLError(const std::string &message = "No Message");
    };
}