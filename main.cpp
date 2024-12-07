#include "core/yrenderer.h"
#include "core/yshader.h"

int main() {
    core::YRenderer renderer(800, 600, "YRenderer");
    renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

#pragma region test_scene
    auto vertexShader =
        "#version 400\n"
        "layout (location = 0) in vec3 vertex;"
          "layout (location = 1) in vec3 normal;"
          "layout (location = 2) in vec2 uv;"
        "uniform mat4 mvpMatrix;"
        "out vec4 nn;"
        "void main() {"
        "  nn = vec4(normal, 1.0);"
        "  gl_Position = mvpMatrix * vec4(vertex, 1.0);"
        "}";

    auto fragmentShader =
    "#version 400\n"
    "in vec4 nn;"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(nn.xyz, 1.0);"
    "}";

    auto vertexProgram = core::YShader::generateShader(vertexShader, GL_VERTEX_SHADER);
    auto fragmentProgram = core::YShader::generateShader(fragmentShader, GL_FRAGMENT_SHADER);
    auto shaderProgram = core::YShader::generateProgram(vertexProgram, fragmentProgram);



#pragma endregion

    do {
        renderer.startFrame();
        renderer.finishFrame();
    } while (renderer.isRunning());
}
