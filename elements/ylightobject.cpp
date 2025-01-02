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
            "    out vectorOut {\n"
            "        vec4 vPosition;\n"
            "    } vectorOut;\n"
            "   uniform mat4 model;\n"
            "   uniform mat4 viewProjection;\n"
            "   void main() {\n"
            "       vectorOut.vPosition = model * vec4(vertex, 1.0);\n"
            "       vec4 vecOut = vectorOut.vPosition;\n"
            "       gl_Position = viewProjection * vecOut;\n"
            "   }\n";

    const std::string YLightObject::defaultGeometryShader =
            "#version 400\n"
            "    layout (triangles) in;\n"
            "    layout (line_strip, max_vertices = 16) out;\n"
            "    in vectorOut {\n"
            "        vec4 vPosition;\n"
            "    } vectorIn[];\n"
            "    uniform vec3 lightDirection;\n"
            "    uniform mat4 viewProjection;\n"
            "    const float MAGNITUDE = 0.4;\n"
            "    const float ARROW_HEAD_SIZE = 0.8;\n"
            "    void main()\n"
            "    {\n"
            "        gl_Position = viewProjection * vectorIn[0].vPosition;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * vectorIn[1].vPosition;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * vectorIn[0].vPosition;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * vectorIn[2].vPosition;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * vectorIn[1].vPosition;\n"
            "        EmitVertex();\n"
            "        gl_Position = viewProjection * vectorIn[2].vPosition;\n"
            "        EmitVertex();\n"
            "        vec3 dir1 = vec3(vectorIn[1].vPosition - vectorIn[0].vPosition);\n"
            "        float hl1 = length(dir1) / 2.0;\n"
            "        dir1 = normalize(dir1);\n"
            "        vec3 dir2 = vec3(vectorIn[2].vPosition - vectorIn[0].vPosition);\n"
            "        float hl2 = length(dir2) / 2.0;\n"
            "        dir2 = normalize(dir2);\n"
            "        vec4 midp = vectorIn[0].vPosition + vec4(dir1*hl1 + dir2*hl2, 0.0);\n"
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
            "       frag_colour = vec4(lightColor.x, lightColor.y, lightColor.z, 1.0);\n"
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

        materialInstance->updateUniformValue(uniformLightColorName, light->getColor());
        materialInstance->updateUniformValue(uniformLightDirectionName, light->getDirection());

        transform.setPosition(light->getPosition());
        transform.scale(glm::vec3(0.001, 0.001, 0.001));
    }

    void YLightObject::draw(const core::YRenderer &renderer) {
        const auto viewProjection = renderer.getCamera()->getCachedViewProjectionMatrix();
        materialInstance->drawModel(transform.getModelMatrix(), lightModel, viewProjection);
    }

    std::shared_ptr<core::YMaterial> YLightObject::getDefaultMaterial() {
        static auto defaultLightObjectMaterial = std::make_shared<core::YMaterial>(
                "defaultLightObjectModel", defaultVertexShader, defaultFragmentShader, defaultGeometryShader);
        defaultLightObjectMaterial->setRenderMode(GL_TRIANGLES);
        return defaultLightObjectMaterial;
    }
} // elements
