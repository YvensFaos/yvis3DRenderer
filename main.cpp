#include "core/ymodel.h"
#include "core/yrenderer.h"
#include "core/yshader.h"
#include "LuaHandler/luahandler.hpp"
#include "utils/yluahelper.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ylight.h"
#include "elements/yfog.h"

int main() {
    constexpr int width = 800;
    constexpr int height = 600;
    core::YRenderer renderer(width, height, "YRenderer");
    renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));
    LuaHandler luaHandler;
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

    core::YCamera &camera = renderer.getCamera();
    utils::YLuaHelper::setupCameraPosition("cameraPosition", camera, luaHandler);

    const GLuint modelMatrixUniform = glGetUniformLocation(shaderProgram, "model");
    const GLuint vpMatrixUniform = glGetUniformLocation(shaderProgram, "viewProjection");
    const GLuint cameraPositionUniform = glGetUniformLocation(shaderProgram, "cameraPosition");
    const GLuint    lightPositionUniform = glGetUniformLocation(shaderProgram, "sceneLight.position");
    const GLuint   lightDirectionUniform = glGetUniformLocation(shaderProgram, "sceneLight.direction");
    const GLuint       lightColorUniform = glGetUniformLocation(shaderProgram, "sceneLight.color");
    const GLuint   lightIntensityUniform = glGetUniformLocation(shaderProgram, "sceneLight.intensity");
    const GLuint lightDirectionalUniform = glGetUniformLocation(shaderProgram, "sceneLight.directional");
    const GLuint sceneFogMaxDistUniform = glGetUniformLocation(shaderProgram, "sceneFog.maxDist");
    const GLuint sceneFogMinDistUniform = glGetUniformLocation(shaderProgram, "sceneFog.minDist");
    const GLuint   sceneFogColorUniform = glGetUniformLocation(shaderProgram, "sceneFog.color");
    auto modelMatrix = glm::mat4(1.0f);

    const core::YModel monkey = utils::YLuaHelper::loadModelFromTable("monkey", luaHandler);
    const core::YLight light = utils::YLuaHelper::loadLightFromTable("light", luaHandler);
    const elements::YFog fog = utils::YLuaHelper::loadFogFromTable("fog", luaHandler);

    glm::vec3 lightPosition = light.getPosition();
    glm::vec3 lightDirection = light.getDirection();
    glm::vec4 lightColor = light.getColor();
    float lightIntensity = light.getIntensity();
    bool lightDirectional = light.getDirectional();
    glm::vec4 fogColor = fog.color;

#pragma endregion

    do {
        renderer.startFrame();
        glUseProgram(shaderProgram);

        const glm::mat4 view = camera.getView();
        const glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),
                                                      static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                                      1000.0f);
        const glm::mat4 viewProjection = projection * view;
        modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f) * renderer.getDeltaTime(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(vpMatrixUniform, 1, GL_FALSE, glm::value_ptr(viewProjection));
        glUniformMatrix4fv(static_cast<GLint>(modelMatrixUniform), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform3f(cameraPositionUniform, camera.getPos().x, camera.getPos().y, camera.getPos().z);
        glUniform3f(lightPositionUniform, lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(lightDirectionUniform, lightDirection.x, lightDirection.y, lightDirection.z);
        glUniform4f(lightColorUniform, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform1f(lightIntensityUniform, lightIntensity);
        glUniform1i(lightDirectionalUniform, lightDirectional);
        glUniform1f(sceneFogMaxDistUniform, fog.maxDist);
        glUniform1f(sceneFogMinDistUniform, fog.minDist);
        glUniform4f(sceneFogColorUniform, fogColor.x, fogColor.y, fogColor.z, fogColor.w);

        monkey.draw(shaderProgram, GL_TRIANGLES, false);

        renderer.finishFrame();
    } while (renderer.isRunning());
}
