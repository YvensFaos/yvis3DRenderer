//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "ymaterialinstance.h"
#include "../utils/yluahelper.h"
#include "yuniformvalue.h"

namespace core {
    YMaterialInstance::YMaterialInstance(std::shared_ptr<YMaterial> &material, const std::string &materialTableName,
                                         LuaHandler &luaHandler) : material(material) {
        auto uniformsIterator = material->getUniformsIterator();
        luaHandler.getTableFromTable(materialTableName);

        while (uniformsIterator != material->getUniformsEndIterator()) {
            auto type = (*uniformsIterator)->getType();
            if ((*uniformsIterator)->getUniformName() == "model") {
                printf("Skipping uniform value generation for the [%s] matrix.\n",
                       (*uniformsIterator)->getUniformName().c_str());
                modelUniformLocation = glGetUniformLocation(material->getProgram(),
                                                            (*uniformsIterator)->getUniformName().c_str());
                GLenum error = glGetError();
                if (error != GL_NO_ERROR) {
                    printf("Error: %d!\n", error);
                }
                ++uniformsIterator;
                continue;
            }
            if ((*uniformsIterator)->getUniformName() == "viewProjection") {
                printf("Skipping uniform value generation for the [%s] matrix.\n",
                       (*uniformsIterator)->getUniformName().c_str());
                viewProjectionUniformLocation = glGetUniformLocation(material->getProgram(),
                                                                     (*uniformsIterator)->getUniformName().c_str());
                GLenum error = glGetError();
                if (error != GL_NO_ERROR) {
                    printf("Error: %d!\n", error);
                }
                ++uniformsIterator;
                continue;
            }
            switch (type) {
                case INT: {
                    auto value = luaHandler.getIntegerFromTable((*uniformsIterator)->getUniformName());
                    uniformValues.push_back(std::make_shared<YUniformValue<int>>(*uniformsIterator, value));
                }
                    break;
                case FLOAT: {
                    auto value = luaHandler.getNumberFromTable((*uniformsIterator)->getUniformName());
                    uniformValues.push_back(std::make_shared<YUniformValue<float>>(*uniformsIterator, value));
                }
                    break;
                case VEC2: {
                    auto value = utils::YLuaHelper::readVec2FromTableInTable((*uniformsIterator)->getUniformName(),
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec2>>(*uniformsIterator, value));
                }
                    break;
                case VEC3: {
                    auto value = utils::YLuaHelper::readVec3FromTableInTable((*uniformsIterator)->getUniformName(),
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec3>>(*uniformsIterator, value));
                }
                    break;
                case VEC4: {
                    auto value = utils::YLuaHelper::readVec4FromTableInTable((*uniformsIterator)->getUniformName(),
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec4>>(*uniformsIterator, value));
                }
                    break;
                case MAT3: {
                    auto value = utils::YLuaHelper::readMat3FromTableInTable((*uniformsIterator)->getUniformName(),
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat3>>(*uniformsIterator, value));
                }
                    break;
                case MAT4: {
                    auto value = utils::YLuaHelper::readMat4FromTableInTable((*uniformsIterator)->getUniformName(),
                                                                             luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat4>>(*uniformsIterator, value));
                }
                    break;
                case BOOL: {
                    auto value = luaHandler.getIntegerFromTable((*uniformsIterator)->getUniformName());
                    uniformValues.push_back(std::make_shared<YUniformValue<bool>>(*uniformsIterator, value != 0));
                }
                    break;
                case SAMPLER2D: {
                    auto value = luaHandler.getIntegerFromTable((*uniformsIterator)->getUniformName());
                    uniformValues.push_back(std::make_shared<YUniformValue<int>>(*uniformsIterator, value));
                }
                    break;
                case CUSTOM: {
                    printf("Custom uniform found: %s. Skip Uniform Value generation.",
                           (*uniformsIterator)->getUniformName().c_str());
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

    void YMaterialInstance::drawModel(const glm::mat4 &modelMatrix, const std::shared_ptr<YModel> &model,
                                      const glm::mat4 &viewProjection) const {
        auto program = material->getProgram();
        glUseProgram(program);
        glUniformMatrix4fv(static_cast<int>(modelUniformLocation), 1, GL_FALSE,
                           glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(static_cast<int>(viewProjectionUniformLocation), 1, GL_FALSE,
                           glm::value_ptr(viewProjection));
        for (const auto &uniformValue: uniformValues) {
            uniformValue->stream();
        }
        model->draw(program);
    }
} // core