//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "ymaterialinstance.h"
#include "yuniformvalue.h"
#include "../utils/yluahelper.h"
#include "../utils/yglhelper.h"

namespace core {
    YMaterialInstance::YMaterialInstance(const std::shared_ptr<YMaterial> &material, const std::string &materialTableName,
                                         const LuaHandler &luaHandler) : material(material) {
        auto uniformsIterator = material->getUniformsIterator();
        if(!luaHandler.getTableFromTable(materialTableName)) {
            printf("Error while reading material table %s. Aborting.\n", materialTableName.c_str());
            return;
        }

        while (uniformsIterator != material->getUniformsEndIterator()) {
            const auto type = (*uniformsIterator)->getType();
            auto identifier = (*uniformsIterator)->getUniformName();
#pragma region unique identifiers
            if (identifier == "model") {
                printf("Skipping uniform value generation for the [%s] matrix.\n",
                       identifier.c_str());
                modelUniformLocation = glGetUniformLocation(material->getProgram(),
                                                            identifier.c_str());
                utils::YGLHelper::checkGLError();
                ++uniformsIterator;
                continue;
            }
            if (identifier == "viewProjection") {
                printf("Skipping uniform value generation for the [%s] matrix.\n",
                       identifier.c_str());
                viewProjectionUniformLocation = glGetUniformLocation(material->getProgram(),
                                                                     identifier.c_str());
                utils::YGLHelper::checkGLError();
                ++uniformsIterator;
                continue;
            }
            if (identifier == "numberPointLights") {
                printf("Skipping uniform value generation for the [%s] numberPointLights.\n",
                       identifier.c_str());
                numberPointLightsLocation = glGetUniformLocation(material->getProgram(),
                                                            identifier.c_str());
                utils::YGLHelper::checkGLError();
                ++uniformsIterator;
                continue;
            }
            if (identifier == "numberDirectionLights") {
                printf("Skipping uniform value generation for the [%s] numberDirectionLights.\n",
                       identifier.c_str());
                numberDirectionLightsLocation = glGetUniformLocation(material->getProgram(),
                                                            identifier.c_str());
                utils::YGLHelper::checkGLError();
                ++uniformsIterator;
                continue;
            }
#pragma endregion

            switch (type) {
                case INT: {
                    auto value = luaHandler.getIntegerFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<int>>(*uniformsIterator, value));
                }
                    break;
                case FLOAT: {
                    auto value = luaHandler.getNumberFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<float>>(*uniformsIterator, value));
                }
                    break;
                case VEC2: {
                    auto value = utils::YLuaHelper::readVec2FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec2>>(*uniformsIterator, value));
                }
                    break;
                case VEC3: {
                    auto value = utils::YLuaHelper::readVec3FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec3>>(*uniformsIterator, value));
                }
                    break;
                case VEC4: {
                    auto value = utils::YLuaHelper::readVec4FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec4>>(*uniformsIterator, value));
                }
                    break;
                case MAT3: {
                    auto value = utils::YLuaHelper::readMat3FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat3>>(*uniformsIterator, value));
                }
                    break;
                case MAT4: {
                    auto value = utils::YLuaHelper::readMat4FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat4>>(*uniformsIterator, value));
                }
                    break;
                case BOOL: {
                    const auto value = luaHandler.getIntegerFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<bool>>(*uniformsIterator, value != 0));
                }
                    break;
                case SAMPLER2D: {
                    auto value = luaHandler.getIntegerFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<int>>(*uniformsIterator, value));
                }
                    break;
                case CUSTOM: {
                    printf("Custom uniform found: %s. Skip Uniform Value generation.\n",
                           identifier.c_str());
                }
                    break;
            }
            ++uniformsIterator;
        }

        luaHandler.popTable();
    }

    void YMaterialInstance::drawModel(const std::shared_ptr<YModel> &model, const glm::mat4 &viewProjection) const {
        drawModel(model->getModelMatrix(), model, viewProjection);
    }

    void YMaterialInstance::addUniformValue(const std::shared_ptr<YBaseUniformValue>& uniformValue) {
        uniformValues.push_back(uniformValue);
    }

    bool YMaterialInstance::doesSupportLight() const {
        return material->doesSupportLight();
    }

    void YMaterialInstance::updateNumberOfLights(const int numberPointLights, const int numberDirectionalLights) {
        this->numberPointLights = numberPointLights;
        this->numberDirectionLights = numberDirectionalLights;
    }

    void YMaterialInstance::drawModel(const glm::mat4 &modelMatrix, const std::shared_ptr<YModel> &model,
                                      const glm::mat4 &viewProjection) const {
        const auto program = material->getProgram();
        glUseProgram(program);
        glUniformMatrix4fv(static_cast<int>(modelUniformLocation), 1, GL_FALSE,
                           glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(static_cast<int>(viewProjectionUniformLocation), 1, GL_FALSE,
                           glm::value_ptr(viewProjection));

        if(material->doesSupportLight()) {
            glUniform1i(static_cast<int>(numberPointLightsLocation), numberPointLights);
            glUniform1i(static_cast<int>(numberDirectionLightsLocation), numberDirectionLights);
        }

        for (const auto &uniformValue: uniformValues) {
            uniformValue->stream();
        }
        model->draw(program);
    }
} // core