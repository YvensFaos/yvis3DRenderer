//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include <GL/glew.h>
#include <string>
#include <vector>
#include "yuniform.h"

namespace core {

    class YMaterial {
    private:
        GLuint shaderProgram;
        std::vector<core::YUniform> uniforms;

    public:
        YMaterial(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader = "");
        ~YMaterial() = default;
    };

} // core
