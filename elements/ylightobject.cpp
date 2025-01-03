//
// Created by Yvens Serpa on 02/01/2025.
//

#include "ylightobject.h"

#include "../core/ylight.h"
#include "../core/ymaterial.h"
#include "../core/ymaterialinstance.h"
#include "../core/yuniformvalue.h"
#include "../core/yrenderer.h"

namespace elements {
#pragma region YLight Default Shaders
    const std::string YLightObject::defaultVertexShader =
            "#version 400\n"
            "   layout (location = 0) in vec3 vertex;\n"
            "   out vectorOut {\n"
            "       vec4 lposition;\n"
            "   } vectorOut;\n"
            "   uniform mat4 model;\n"
            "   uniform mat4 viewProjection;\n"
            "   void main()\n"
            "   {\n"
            "       vectorOut.lposition = model * vec4(vertex, 1.0);\n"
            "       gl_Position = viewProjection * vectorOut.lposition;\n"
            "   }\n";

    const std::string YLightObject::defaultGeometryShader =
            "#version 400\n"
            "    layout (triangles) in;\n"
            "    layout (line_strip, max_vertices = 10) out;\n"
            "    in vectorOut {\n"
            "        vec4 lposition;\n"
            "    } vectorIn[];\n"
            "    uniform vec3 lightDirection;\n"
            "    uniform mat4 viewProjection;\n"
            "    const float MAGNITUDE = 0.2;\n"
            "    const float ARROW_HEAD_SIZE = 0.4;\n"
            "    void main()\n"
            "    {\n"
            "        vec3 dir1 = (vec3(vectorIn[1].lposition - vectorIn[0].lposition));\n"
            "        float hl1 = length(dir1) / 2.0;\n"
            "        dir1 = normalize(dir1);\n"
            "        vec3 dir2 = (vec3(vectorIn[2].lposition - vectorIn[0].lposition));\n"
            "        float hl2 = length(dir2) / 2.0;\n"
            "        dir2 = normalize(dir2);\n"
            "        vec4 midp = vectorIn[0].lposition + vec4(dir1*hl1 + dir2*hl2, 0.0);\n"
            "        gl_Position = viewProjection * midp;\n"
            "        EmitVertex();\n"
            "        vec3 normalizedLightDirection = normalize(lightDirection);\n"
            "        vec3 direction = vec3(midp) - normalizedLightDirection;\n"
            "        direction = normalize(direction);\n"
            "        vec4 finp = midp + vec4(normalizedLightDirection, 0) * MAGNITUDE;\n"
            "        gl_Position = viewProjection * finp;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp + vec4(cross(vec3(1, 0, 0), direction), 0) * MAGNITUDE * ARROW_HEAD_SIZE;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp + vec4(cross(vec3(-1, 0, 0), direction), 0) * MAGNITUDE * ARROW_HEAD_SIZE;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp + vec4(cross(vec3(0, 1, 0), direction), 0) * MAGNITUDE * ARROW_HEAD_SIZE;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp + vec4(cross(vec3(0, -1, 0), direction), 0) * MAGNITUDE * ARROW_HEAD_SIZE;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp + vec4(cross(vec3(0, 0, 1), direction), 0) * MAGNITUDE * ARROW_HEAD_SIZE;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * finp + vec4(cross(vec3(0, 0, -1), direction), 0) * MAGNITUDE * ARROW_HEAD_SIZE;\n"
            "        EmitVertex();\n"
            "        EndPrimitive();\n"
            "    }\n";

    const std::string YLightObject::defaultFragmentShader =
            "#version 400\n"
            "   uniform vec4 lightColor;\n"
            "   out vec4 frag_colour;\n"
            "   void main()\n"
            "   {\n"
            "       frag_colour = lightColor;\n"
            "   }\n";
#pragma endregion

    const std::string YLightObject::defaultLightModelPath = "data/models/tetrahedron.fbx";

    YLightObject::YLightObject(const std::string &identifier,
                               const std::shared_ptr<core::YLight> &light) : YObject(identifier), light(light),
                                                                             lightModel(std::make_shared<core::YModel>(
                                                                                 defaultLightModelPath)) {
        materialInstance = std::make_shared<core::YMaterialInstance>(YLightObject::getDefaultMaterial());

        const auto uniformLightColorName = "lightColor";
        const auto uniformLightDirectionName = "lightDirection";

        auto lightColorUniform = YLightObject::getDefaultMaterial()->getUniform(uniformLightColorName);
        materialInstance->updateUniformValue(uniformLightColorName, std::make_shared<core::YUniformValue<glm::vec4>>(lightColorUniform, light->getColor()));
        auto lightDirectionUniform = YLightObject::getDefaultMaterial()->getUniform(uniformLightDirectionName);
        materialInstance->updateUniformValue(uniformLightDirectionName, std::make_shared<core::YUniformValue<glm::vec3>>(lightColorUniform, light->getDirection()));

        transform.setPosition(light->getPosition());
    }

    void YLightObject::draw(const core::YRenderer &renderer) {
        YObject::draw(renderer);
        materialInstance->drawModel(transform.getModelMatrix(), lightModel,
                                    renderer.getCamera()->getCachedViewProjectionMatrix());
    }

    std::shared_ptr<core::YMaterial> YLightObject::getDefaultMaterial() {
        static auto defaultLightObjectMaterial = std::make_shared<core::YMaterial>("defaultLightObjectModel", defaultVertexShader, defaultFragmentShader,
                                                                                   defaultGeometryShader);
        return defaultLightObjectMaterial;
    }
} // elements
