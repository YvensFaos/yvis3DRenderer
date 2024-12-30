//
// Created by Yvens Serpa on 19/12/2024.
//

#include "yedgedetectionscene.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../core/yshader.h"
#include "../utils/yluahelper.h"

namespace scenes {
    YEdgeDetectionScene::YEdgeDetectionScene(core::YRenderer &renderer, const std::string &file, const int width,
                                             const int height) : YScene(renderer, file, width, height),
                                                                 edgeQuad(luaHandler.getGlobalString(
                                                                     "edgeFragmentShader")),
                                                                 edgeBuffer(static_cast<GLfloat>(width) * 2,
                                                                            static_cast<GLfloat>(height) * 2),
                                                                 light(utils::YLuaHelper::loadLightFromTable(
                                                                     "light", luaHandler)), modelMatrix(1) {
        renderer.changeClearColor(glm::vec4(1.0f, 0.2f, 0.3f, 1.0f));

        const auto vertexShader = luaHandler.getGlobalString("vertexShader");
        const auto fragmentShader = luaHandler.getGlobalString("fragmentShader");
        const auto vertexProgram = core::YShader::generateShader(vertexShader, GL_VERTEX_SHADER);
        const auto fragmentProgram = core::YShader::generateShader(fragmentShader, GL_FRAGMENT_SHADER);
        shaderProgram = core::YShader::generateProgram(vertexProgram, fragmentProgram);

        const auto camera = renderer.getCamera();

        models = utils::YLuaHelper::loadModelsFromTable("models", luaHandler);

        utils::YLuaHelper::setupCameraPosition("cameraPosition", camera, luaHandler);

        uniforms.emplace("modelMatrixUniform", glGetUniformLocation(shaderProgram, "model"));
        uniforms.emplace("vpMatrixUniform", glGetUniformLocation(shaderProgram, "viewProjection"));
        uniforms.emplace("lightViewProjectionUniform", glGetUniformLocation(shaderProgram, "lightViewProjection"));
        uniforms.emplace("lightPositionUniform", glGetUniformLocation(shaderProgram, "sceneLight.position"));
        uniforms.emplace("lightDirectionUniform", glGetUniformLocation(shaderProgram, "sceneLight.direction"));
        uniforms.emplace("lightColorUniform", glGetUniformLocation(shaderProgram, "sceneLight.color"));
        uniforms.emplace("lightIntensityUniform", glGetUniformLocation(shaderProgram, "sceneLight.intensity"));
        uniforms.emplace("lightDirectionalUniform", glGetUniformLocation(shaderProgram, "sceneLight.directional"));
    }

    void YEdgeDetectionScene::renderImpl() {
        glUseProgram(shaderProgram);

        const auto camera = renderer.getCamera();
        const glm::mat4 view = camera->getView();
        const glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()),
                                                      static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                                      1000.0f);
        glm::mat4 viewProjectionMatrix = projection * view;

        edgeBuffer.bindBuffer();
        glEnable(GL_DEPTH_TEST);
        edgeBuffer.setViewport();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_CULL_FACE);
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(uniforms["vpMatrixUniform"], 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
        glUniform3f(uniforms["lightPositionUniform"], light.getPosition().x, light.getPosition().y,
                    light.getPosition().z);
        glUniform3f(uniforms["lightDirectionUniform"], light.getDirection().x, light.getDirection().y,
                    light.getDirection().z);
        glUniform4f(uniforms["lightColorUniform"], light.getColor().x, light.getColor().y, light.getColor().z,
                    light.getColor().w);
        glUniform1f(uniforms["lightIntensityUniform"], light.getIntensity());
        glUniform1i(uniforms["lightDirectionalUniform"], light.getDirectional());

        core::YModel::renderModelsInList(models, uniforms["modelMatrixUniform"], shaderProgram);
        edgeBuffer.unbindBuffer();

        edgeBuffer.unbindBuffer();
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        edgeQuad.render(edgeBuffer.getFramebufferTexture());

        for (auto &model: models) {
            model.rotate(glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }
} // scenes
