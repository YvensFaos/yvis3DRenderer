//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#include "ymaterialinstance.h"
#include "../utils/yluahelper.h"

namespace core {
    YMaterialInstance::YMaterialInstance(const YMaterial &material, LuaHandler &luaHandler) : material(material) {
        auto uniformsIterator = material.getUniformsIterator();

        while (uniformsIterator != material.getUniformsEndIterator()) {
            auto type = uniformsIterator->type;
            switch (type) {
                case INT: {
                    auto value = luaHandler.getIntegerFromTable(uniformsIterator->uniformName);
                    uniformValues.push_back(new YUniformValue<int>(*uniformsIterator, value));
                }
                    break;
                case FLOAT: {
                    auto value = luaHandler.getNumberFromTable(uniformsIterator->uniformName);
                    uniformValues.push_back(new YUniformValue<float>(*uniformsIterator, value));
                }
                    break;
                case VEC2: {
                    auto value = utils::YLuaHelper::readVec2FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(new YUniformValue<glm::vec2>(*uniformsIterator, value));
                }
                    break;
                case VEC3: {
                    auto value = utils::YLuaHelper::readVec3FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(new YUniformValue<glm::vec3>(*uniformsIterator, value));
                }
                    break;
                case VEC4: {
                    auto value = utils::YLuaHelper::readVec4FromTableInTable(uniformsIterator->uniformName, luaHandler);
                    uniformValues.push_back(new YUniformValue<glm::vec4>(*uniformsIterator, value));
                }
                    break;
                case MAT3:
                    break;
                case MAT4:
                    break;
                case BOOL:
                    break;
                case SAMPLER2D:
                    break;
                case CUSTOM:
                    break;
            }
        }
    }
} // core