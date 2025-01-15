//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "ymaterialinstance.h"

#include <glm/gtc/type_ptr.hpp>

#include "ycamera.h"
#include "ymaterial.h"
#include "yuniformvalue.h"
#include "../utils/yluahelper.h"
#include "../utils/yglhelper.h"

namespace core {
    YMaterialInstance::YMaterialInstance(const std::shared_ptr<YMaterial> &material) : modelUniformLocation(-1),
        viewProjectionUniformLocation(
            -1),
        numberPointLightsLocation(-1),
        numberDirectionLightsLocation(
            -1),
        cameraPositionUniformLocation(-1),
        numberPointLights(-1),
        numberDirectionLights(-1),
        material(material) {
        auto uniformsIterator = material->getUniformsIterator();

        while (uniformsIterator != material->getUniformsEndIterator()) {
            const auto identifier = (*uniformsIterator)->getUniformName();
            if (getModelUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getViewProjectionUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getCameraPositionUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getNumberPointLightsUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getNumberDirectionLightsUniformLocation(material, uniformsIterator, identifier)) continue;
            switch ((*uniformsIterator)->getType()) {
                case INT: {
                    uniformValues.push_back(std::make_shared<YUniformValue<int> >(*uniformsIterator, 0));
                }
                break;
                case FLOAT: {
                    uniformValues.push_back(std::make_shared<YUniformValue<float> >(*uniformsIterator, 0.0f));
                }
                break;
                case VEC2: {
                    uniformValues.push_back(
                        std::make_shared<YUniformValue<glm::vec2> >(*uniformsIterator, glm::vec2(0.0f)));
                }
                break;
                case VEC3: {
                    uniformValues.push_back(
                        std::make_shared<YUniformValue<glm::vec3> >(*uniformsIterator, glm::vec3(0.0f)));
                }
                break;
                case VEC4: {
                    uniformValues.push_back(
                        std::make_shared<YUniformValue<glm::vec4> >(*uniformsIterator, glm::vec4(0.0f)));
                }
                break;
                case MAT3: {
                    uniformValues.push_back(
                        std::make_shared<YUniformValue<glm::mat3> >(*uniformsIterator, glm::mat3(0.0f)));
                }
                break;
                case MAT4: {
                    uniformValues.push_back(
                        std::make_shared<YUniformValue<glm::mat4> >(*uniformsIterator, glm::mat4(0.0f)));
                }
                break;
                case BOOL: {
                    uniformValues.push_back(std::make_shared<YUniformValue<bool> >(*uniformsIterator, false));
                }
                break;
                case SAMPLER2D: {
                    uniformValues.push_back(std::make_shared<YUniformValue<int> >(*uniformsIterator, -1));
                }
                break;
                case CUSTOM: {
                    printf("Custom uniform found: %s.\nSkip Uniform Value generation.\n",
                           identifier.c_str());
                }
                break;
            }
            ++uniformsIterator;
        }
    }

    YMaterialInstance::YMaterialInstance(const std::shared_ptr<YMaterial> &material,
                                         const std::string &materialTableName,
                                         const LuaHandler &luaHandler) : modelUniformLocation(-1), viewProjectionUniformLocation(-1),
                                                                         numberPointLightsLocation(-1),
                                                                            numberDirectionLightsLocation(-1),
                                                                         cameraPositionUniformLocation(-1),
                                                                         numberPointLights(-1),
                                                                         numberDirectionLights(-1),
                                                                         material(material) {
        auto uniformsIterator = material->getUniformsIterator();
        if (!luaHandler.getTableFromTable(materialTableName)) {
            printf("Error while reading material table %s. Aborting.\n", materialTableName.c_str());
            return;
        }

        while (uniformsIterator != material->getUniformsEndIterator()) {
            const auto type = (*uniformsIterator)->getType();
            auto identifier = (*uniformsIterator)->getUniformName();
#pragma region unique identifiers
            if (getModelUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getViewProjectionUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getCameraPositionUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getNumberPointLightsUniformLocation(material, uniformsIterator, identifier)) continue;
            if (getNumberDirectionLightsUniformLocation(material, uniformsIterator, identifier)) continue;
#pragma endregion

            switch (type) {
                case INT: {
                    auto value = luaHandler.getIntegerFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<int> >(*uniformsIterator, value));
                }
                break;
                case FLOAT: {
                    auto value = luaHandler.getNumberFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<float> >(*uniformsIterator, value));
                }
                break;
                case VEC2: {
                    auto value = utils::YLuaHelper::readVec2FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec2> >(*uniformsIterator, value));
                }
                break;
                case VEC3: {
                    auto value = utils::YLuaHelper::readVec3FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec3> >(*uniformsIterator, value));
                }
                break;
                case VEC4: {
                    auto value = utils::YLuaHelper::readVec4FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec4> >(*uniformsIterator, value));
                }
                break;
                case MAT3: {
                    auto value = utils::YLuaHelper::readMat3FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat3> >(*uniformsIterator, value));
                }
                break;
                case MAT4: {
                    auto value = utils::YLuaHelper::readMat4FromTableInTable(identifier,
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat4> >(*uniformsIterator, value));
                }
                break;
                case BOOL: {
                    const auto value = luaHandler.getIntegerFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<bool> >(*uniformsIterator, value != 0));
                }
                break;
                case SAMPLER2D: {
                    auto value = luaHandler.getIntegerFromTable(identifier);
                    uniformValues.push_back(std::make_shared<YUniformValue<int> >(*uniformsIterator, value));
                }
                break;
                case CUSTOM: {
                    printf("Custom uniform found: %s.\nSkip Uniform Value generation.\n",
                           identifier.c_str());
                }
                break;
            }
            ++uniformsIterator;
        }

        luaHandler.popTable();
    }

    void YMaterialInstance::drawModel(const std::shared_ptr<YCamera>& camera, const std::shared_ptr<YModel> &model, const glm::mat4 &viewProjection) const {
        drawModel(camera, model->getModelMatrix(), model, viewProjection);
    }

    void YMaterialInstance::addUniformValue(const std::shared_ptr<YBaseUniformValue> &uniformValue) {
        uniformValues.push_back(uniformValue);
    }

    bool YMaterialInstance::doesSupportLight() const {
        return material->doesSupportLight();
    }

    void YMaterialInstance::updateNumberOfLights(const int pointLights, const int numberDirectionalLights) {
        this->numberPointLights = pointLights;
        this->numberDirectionLights = numberDirectionalLights;
    }

    void YMaterialInstance::drawModel(const std::shared_ptr<YCamera>& camera,
                const glm::mat4 &modelMatrix, const std::shared_ptr<YModel> &model,
                                      const glm::mat4 &viewProjection) const {
        const auto program = material->getProgram();
        glUseProgram(program);
        glUniformMatrix4fv(static_cast<int>(modelUniformLocation), 1, GL_FALSE,
                           glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(static_cast<int>(viewProjectionUniformLocation), 1, GL_FALSE,
                           glm::value_ptr(viewProjection));
        glUniform3fv(static_cast<int>(cameraPositionUniformLocation), 1, &camera->getPos()[0]);

        if (material->doesSupportLight()) {
            glUniform1i(static_cast<int>(numberPointLightsLocation), numberPointLights);
            glUniform1i(static_cast<int>(numberDirectionLightsLocation), numberDirectionLights);
        }

        for (const auto &uniformValue: uniformValues) {
            uniformValue->stream();
        }
        model->draw(program, material->getRenderMode());
    }

    bool YMaterialInstance::getModelUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                    std::vector<std::shared_ptr<core::YUniform> >::const_iterator &
                                                    uniformsIterator, const std::string &identifier) {
        if (identifier == "model") {
            printf("Skipping uniform value generation for the [%s] matrix.\n",
                   identifier.c_str());
            modelUniformLocation = glGetUniformLocation(yMaterial->getProgram(),
                                                        identifier.c_str());
            utils::YGLHelper::checkGLError();
            ++uniformsIterator;
            return true;
        }
        return false;
    }

    bool YMaterialInstance::getViewProjectionUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                             std::vector<std::shared_ptr<
                                                                 core::YUniform> >::const_iterator &uniformsIterator,
                                                             const std::string &identifier) {
        if (identifier == "viewProjection") {
            printf("Skipping uniform value generation for the [%s] matrix.\n",
                   identifier.c_str());
            viewProjectionUniformLocation = glGetUniformLocation(yMaterial->getProgram(),
                                                                 identifier.c_str());
            utils::YGLHelper::checkGLError();
            ++uniformsIterator;
            return true;
        }
        return false;
    }

    bool YMaterialInstance::getCameraPositionUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                             std::vector<std::shared_ptr<
                                                                 core::YUniform> >::const_iterator &uniformsIterator,
                                                             const std::string &identifier) {
        if (identifier == "cameraPosition") {
            printf("Skipping uniform value generation for the [%s] cameraPosition.\n",
                   identifier.c_str());
            cameraPositionUniformLocation = glGetUniformLocation(yMaterial->getProgram(),
                                                                 identifier.c_str());
            utils::YGLHelper::checkGLError();
            ++uniformsIterator;
            return true;
        }
        return false;
    }

    bool YMaterialInstance::getNumberPointLightsUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                                std::vector<std::shared_ptr<
                                                                    core::YUniform> >::const_iterator &uniformsIterator,
                                                                const std::string &identifier) {
        if (identifier == "numberPointLights") {
            printf("Skipping uniform value generation for the [%s] numberPointLights.\n",
                   identifier.c_str());
            numberPointLightsLocation = glGetUniformLocation(yMaterial->getProgram(),
                                                             identifier.c_str());
            utils::YGLHelper::checkGLError();
            ++uniformsIterator;
            return true;
        }
        return false;
    }

    bool YMaterialInstance::getNumberDirectionLightsUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                                    std::vector<std::shared_ptr<
                                                                        core::YUniform> >::const_iterator &
                                                                    uniformsIterator, const std::string &identifier) {
        if (identifier == "numberDirectionLights") {
            printf("Skipping uniform value generation for the [%s] numberDirectionLights.\n",
                   identifier.c_str());
            numberDirectionLightsLocation = glGetUniformLocation(yMaterial->getProgram(),
                                                                 identifier.c_str());
            utils::YGLHelper::checkGLError();
            ++uniformsIterator;
            return true;
        }
        return false;
    }

    template<typename T>
    void YMaterialInstance::updateUniformValue(const std::string &uniformName, T uniformValue) {
        printf("Error! Uniform %s being updated without a valid specialized template method.\n", uniformName.c_str());
    }

    template<>
    void YMaterialInstance::updateUniformValue(const std::string &uniformName, glm::vec3 uniformValue) {
        auto uniform = findUniform(uniformName);
        if (uniform != nullptr) {
            printf("1 - Uniform %s found.\n", uniformName.c_str());
            auto derived = std::dynamic_pointer_cast<YUniformValue<glm::vec3> >(uniform);
            if (derived != nullptr) {
                printf("2 - Updating uniform after successful dynamic_cast.\n");
                derived->set(uniformValue);
            } else {
                printf("2 - Failed to update uniform after unsuccessful dynamic_cast.\n");
            }
        } else {
            printf("1 - Error! Uniform %s was NOT found.\n", uniformName.c_str());
        }
    }

    template<>
    void YMaterialInstance::updateUniformValue(const std::string &uniformName, glm::vec4 uniformValue) {
        auto uniform = findUniform(uniformName);
        if (uniform != nullptr) {
            printf("1 - Uniform %s found.\n", uniformName.c_str());
            auto derived = std::dynamic_pointer_cast<YUniformValue<glm::vec4> >(uniform);
            if (derived != nullptr) {
                printf("2 - Updating uniform after successful dynamic_cast.\n");
                derived->set(uniformValue);
            } else {
                printf("2 - Failed to update uniform after unsuccessful dynamic_cast.\n");
            }
        } else {
            printf("1 - Error! Uniform %s was NOT found.\n", uniformName.c_str());
        }
    }

    std::shared_ptr<YBaseUniformValue>
    YMaterialInstance::findUniform(const std::string &uniformName) {
        const auto found = std::find_if(uniformValues.begin(), uniformValues.end(),
                                        [&uniformName](
                                    const std::__1::shared_ptr<YBaseUniformValue> &baseUniformValue) -> bool {
                                            return baseUniformValue->getUniformName() == uniformName;
                                        });
        if (found != uniformValues.end()) {
            return *found;
        }
        return nullptr;
    }

    std::shared_ptr<core::YMaterial> &YMaterialInstance::getMaterial() {
        return material;
    }
} // core
