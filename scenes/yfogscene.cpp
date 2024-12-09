//
// Created by Yvens Serpa on 09/12/2024.
//

#include "yfogscene.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "../core/yshader.h"
#include "../core/ylight.h"
#include "../elements/yfog.h"
#include "../utils/yluahelper.h"

namespace scenes {
    YFogScene::YFogScene(core::YRenderer &renderer, const std::string &file, const int width,
                         const int height) : YScene(renderer, file, width, height),
                                             monkey(
                                                 utils::YLuaHelper::loadModelFromTable(
                                                     "monkey", luaHandler)),
                                             light(
                                                 utils::YLuaHelper::loadLightFromTable(
                                                     "light", luaHandler)),
                                             fog(utils::YLuaHelper::loadFogFromTable(
                                                 "fog", luaHandler)), modelMatrix(glm::mat4(1.0f)) {
        renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

        const auto vertexShader = luaHandler.getGlobalString("vertexShader");
        const auto fragmentShader = luaHandler.getGlobalString("fragmentShader");
        const auto vertexProgram = core::YShader::generateShader(vertexShader, GL_VERTEX_SHADER);
        const auto fragmentProgram = core::YShader::generateShader(fragmentShader, GL_FRAGMENT_SHADER);
        shaderProgram = core::YShader::generateProgram(vertexProgram, fragmentProgram);

        core::YCamera &camera = renderer.getCamera();
        utils::YLuaHelper::setupCameraPosition("cameraPosition", camera, luaHandler);

        uniforms.emplace("modelMatrixUniform", glGetUniformLocation(shaderProgram, "model"));
        uniforms.emplace("vpMatrixUniform", glGetUniformLocation(shaderProgram, "viewProjection"));
        uniforms.emplace("cameraPositionUniform", glGetUniformLocation(shaderProgram, "cameraPosition"));
        uniforms.emplace("lightPositionUniform", glGetUniformLocation(shaderProgram, "sceneLight.position"));
        uniforms.emplace("lightDirectionUniform", glGetUniformLocation(shaderProgram, "sceneLight.direction"));
        uniforms.emplace("lightColorUniform", glGetUniformLocation(shaderProgram, "sceneLight.color"));
        uniforms.emplace("lightIntensityUniform", glGetUniformLocation(shaderProgram, "sceneLight.intensity"));
        uniforms.emplace("lightDirectionalUniform", glGetUniformLocation(shaderProgram, "sceneLight.directional"));
        uniforms.emplace("sceneFogMaxDistUniform", glGetUniformLocation(shaderProgram, "sceneFog.maxDist"));
        uniforms.emplace("sceneFogMinDistUniform", glGetUniformLocation(shaderProgram, "sceneFog.minDist"));
        uniforms.emplace("sceneFogColorUniform", glGetUniformLocation(shaderProgram, "sceneFog.color"));
    }

    void YFogScene::renderImpl() {
        glUseProgram(shaderProgram);

        const auto camera = renderer.getCamera();
        const glm::mat4 view = camera.getView();
        const glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),
                                                      static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                                      1000.0f);
        const glm::mat4 viewProjection = projection * view;
        modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f) * renderer.getDeltaTime(),
                                  glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(uniforms["vpMatrixUniform"], 1, GL_FALSE, glm::value_ptr(viewProjection));
        glUniformMatrix4fv(static_cast<GLint>(uniforms["modelMatrixUniform"]), 1, GL_FALSE,
                           glm::value_ptr(modelMatrix));
        glUniform3f(uniforms["cameraPositionUniform"], camera.getPos().x, camera.getPos().y, camera.getPos().z);
        auto lightPosition = light.getPosition();
        glUniform3f(uniforms["lightPositionUniform"], lightPosition.x, lightPosition.y, lightPosition.z);
        auto lightDirection = light.getDirection();
        glUniform3f(uniforms["lightDirectionUniform"], lightDirection.x, lightDirection.y, lightDirection.z);
        auto lightColor = light.getColor();
        glUniform4f(uniforms["lightColorUniform"], lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform1f(uniforms["lightIntensityUniform"], light.getIntensity());
        glUniform1i(uniforms["lightDirectionalUniform"], light.getDirectional());
        glUniform1f(uniforms["sceneFogMaxDistUniform"], fog.maxDist);
        glUniform1f(uniforms["sceneFogMinDistUniform"], fog.minDist);
        glUniform4f(uniforms["sceneFogColorUniform"], fog.color.x, fog.color.y, fog.color.z, fog.color.w);

        monkey.draw(shaderProgram, GL_TRIANGLES, false);
    }
} // scenes
