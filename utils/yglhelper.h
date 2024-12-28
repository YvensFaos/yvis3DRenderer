//
// Created by Yvens Serpa on 28/12/2024.
//

#pragma once
#include <string>

namespace utils {
    class YGLHelper {
    public:
        static void checkGLError(const std::string &message);
    };

    void YGLHelper::checkGLError(const std::string &message = "No Message") {
        if (const GLenum error = glGetError(); error != GL_NO_ERROR) {
            printf("Error: %d! [%s]\n", error, message.c_str());
        }
    }
}