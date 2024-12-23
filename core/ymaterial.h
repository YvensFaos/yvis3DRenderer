//
// Created by Yvens Rebouças Serpa on 22/12/2024.
//

#include <GL/glew.h>
#include <string>
#include <vector>
#include "yuniform.h"
#include "ymodel.h"

namespace core {

    class YMaterial {
    private:
        GLuint shaderProgram;
        std::vector<core::YUniform> uniforms;
        GLenum renderMode;

    public:
        YMaterial(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader = "");
        ~YMaterial() = default;

        void drawModel(const YModel& model) const;

        void setRenderMode(GLenum mode);

        [[nodiscard]] std::vector<core::YUniform>::const_iterator getUniformsIterator() const;
        [[nodiscard]] std::vector<core::YUniform>::const_iterator getUniformsEndIterator() const;
    };

} // core
