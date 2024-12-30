//
// Created by Yvens Serpa on 30/12/2024.
//

#include "yglhelper.h"

#include <GL//glew.h>

namespace utils {
    void YGLHelper::checkGLError(const std::string &message) {
        if (const GLenum error = glGetError(); error != GL_NO_ERROR) {
            printf("Error: %d! [%s]\n", error, message.c_str());
        }
    }
}