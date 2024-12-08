#include "core/ymodel.h"
#include "core/yrenderer.h"
#include "core/yshader.h"
#include "LuaHandler/luahandler.hpp"
#include "utils/yluahelper.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YRenderer renderer(width, height, "YRenderer");
    renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));
    const LuaHandler luaHandler;
    if (luaHandler.openFile("data/scenes/scene_simple.lua")) {
        printf("Successfully loaded scene_simple.lua\n");
    } else {
        printf("Failed to load scene_simple.lua\n");
    }

#pragma region test_scene
    const auto vertexShader = luaHandler.getGlobalString("vertexShader");
    const auto fragmentShader = luaHandler.getGlobalString("fragmentShader");
    const auto vertexProgram = core::YShader::generateShader(vertexShader, GL_VERTEX_SHADER);
    const auto fragmentProgram = core::YShader::generateShader(fragmentShader, GL_FRAGMENT_SHADER);
    const auto shaderProgram = core::YShader::generateProgram(vertexProgram, fragmentProgram);

    const core::YCamera &camera = renderer.getCamera();
    const GLint mvpMatrixUniform = glGetUniformLocation(shaderProgram, "mvpMatrix");
    const glm::mat4 view = camera.getView();
    const glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),
                                                  static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                                  1000.0f);
    auto modelMatrix = glm::mat4(1.0f);

    const glm::mat4 viewProjection = projection * view;
    const core::YModel monkey = utils::YLuaHelper::loadModelFromTable("monkey", luaHandler);

#pragma endregion
    do {
        renderer.startFrame();
        glUseProgram(shaderProgram);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f) * renderer.getDeltaTime(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, glm::value_ptr(viewProjection * modelMatrix));
        monkey.draw(shaderProgram, GL_TRIANGLES, false);
        renderer.finishFrame();
    } while (renderer.isRunning());
}
