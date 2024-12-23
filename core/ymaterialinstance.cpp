//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "ymaterialinstance.h"
#include "../utils/yluahelper.h"
#include "yuniformvalue.h"

namespace core {
    YMaterialInstance::YMaterialInstance(std::shared_ptr<YMaterial> &material, const std::string& materialTableName, LuaHandler &luaHandler) : material(material) {
        auto uniformsIterator = material->getUniformsIterator();
        luaHandler.getTableFromTable(materialTableName);

        while (uniformsIterator != material->getUniformsEndIterator()) {
            auto type = uniformsIterator->type;
            switch (type) {
                case INT: {
                    auto value = luaHandler.getIntegerFromTable(uniformsIterator->uniformName);
                    uniformValues.push_back(std::make_shared<YUniformValue<int>>(*uniformsIterator, value));
                }
                    break;
                case FLOAT: {
                    auto value = luaHandler.getNumberFromTable(uniformsIterator->uniformName);
                    uniformValues.push_back(std::make_shared<YUniformValue<float>>(*uniformsIterator, value));
                }
                    break;
                case VEC2: {
                    auto value = utils::YLuaHelper::readVec2FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec2>>(*uniformsIterator, value));
                }
                    break;
                case VEC3: {
                    auto value = utils::YLuaHelper::readVec3FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec3>>(*uniformsIterator, value));
                }
                    break;
                case VEC4: {
                    auto value = utils::YLuaHelper::readVec4FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::vec4>>(*uniformsIterator, value));
                }
                    break;
                case MAT3: {
                    auto value = utils::YLuaHelper::readMat3FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat3>>(*uniformsIterator, value));
                }
                    break;
                case MAT4:{
                    auto value = utils::YLuaHelper::readMat4FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(std::make_shared<YUniformValue<glm::mat4>>(*uniformsIterator, value));
                }
                    break;
                case BOOL:  {
                    auto value = luaHandler.getIntegerFromTable(uniformsIterator->uniformName);
                    uniformValues.push_back(std::make_shared<YUniformValue<bool>>(*uniformsIterator, value != 0));
                }
                    break;
                case SAMPLER2D: {
                    auto value = luaHandler.getIntegerFromTable(uniformsIterator->uniformName);
                    uniformValues.push_back(std::make_shared<YUniformValue<int>>(*uniformsIterator, value));
                }
                    break;
                case CUSTOM: {
                    printf("Custom uniform found: %s. Skip Uniform Value generation.", uniformsIterator->uniformName.c_str());
                }
                    break;
            }
            ++uniformsIterator;
        }
    }

    void YMaterialInstance::drawModel(const std::shared_ptr<YModel> &model, const glm::mat4& viewProject) const {
        for(auto uniformValue : uniformValues) {

        }
    }
} // core