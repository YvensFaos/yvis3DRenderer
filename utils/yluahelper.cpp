//
// Created by Yvens Serpa on 08/12/2024.
//

#include "yluahelper.h"
#include "../core/ymodel.h"
#include "../core/ylight.h"
#include "../core/ycamera.h"
#include "../elements/yfog.h"
#include "../LuaHandler/luahandler.hpp"

namespace utils {
    std::vector<core::YModel> YLuaHelper::loadModelsFromTable(const std::string &identifier,
                                                              const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        const GLuint tableSize = luaHandler.getLength();
        std::vector<core::YModel> models;

        for (int i = 1; i <= tableSize; i++) {
            if (luaHandler.getTableFromTable(i)) {
                models.push_back(YLuaHelper::loadModel(luaHandler, true));
            }
        }
        luaHandler.popTable();
        return models;
    }

    core::YModel YLuaHelper::loadModelFromTable(const std::string &identifier, const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        return loadModel(luaHandler, true);
    }

    core::YModel YLuaHelper::loadModel(const LuaHandler &luaHandler, const bool popTable) {
        core::YModel model(luaHandler.getStringFromTable("file"));

        if (luaHandler.getTableFromTable("pos")) {
            glm::vec3 translateTo;
            translateTo.x = luaHandler.getNumberFromTable(1);
            translateTo.y = luaHandler.getNumberFromTable(2);
            translateTo.z = luaHandler.getNumberFromTable(3);
            model.translate(translateTo);
            luaHandler.popTable();
        }


        if (luaHandler.getTableFromTable("rot")) {
            glm::vec3 rotateTo;
            rotateTo.x = luaHandler.getNumberFromTable(1);
            rotateTo.y = luaHandler.getNumberFromTable(2);
            rotateTo.z = luaHandler.getNumberFromTable(3);
            model.rotate(rotateTo);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("sca")) {
            glm::vec3 scaleTo;
            scaleTo.x = luaHandler.getNumberFromTable(1);
            scaleTo.y = luaHandler.getNumberFromTable(2);
            scaleTo.z = luaHandler.getNumberFromTable(3);
            model.scale(scaleTo);
            luaHandler.popTable();
        }

        if (popTable) {
            luaHandler.popTable();
        }

        return model;
    }

    core::YInstanceMesh YLuaHelper::loadInstanceMeshFromTable(const std::string &identifier,
                                                              const LuaHandler &luaHandler,
                                                              const bool popTable) {
        luaHandler.loadTable(identifier);

        const core::YModel model(luaHandler.getStringFromTable("file"));
        const core::YMesh &singleMesh = model.getMeshAt(0);

        std::vector<glm::mat4> instanceData;
        if (luaHandler.getTableFromTable("instanceData")) {
            const GLuint tableSize = luaHandler.getLength();

            for (int i = 1; i <= tableSize; i++) {
                if (luaHandler.getTableFromTable(i)) {
                    const glm::vec3 pos = readVec3FromTableInTable("pos", luaHandler);
                    const glm::vec3 rot = readVec3FromTableInTable("rot", luaHandler);
                    const glm::vec3 sca = readVec3FromTableInTable("sca", luaHandler);

                    glm::mat4 mat4 = core::YInstanceMesh::fromValuesToInstanceMatrix(pos, rot, sca);
                    instanceData.push_back(mat4);
                    luaHandler.popTable();
                }
            }
            luaHandler.popTable();

            if (popTable) {
                luaHandler.popTable();
            }
        }

        return {singleMesh, instanceData};
    }

    std::vector<core::YLight> YLuaHelper::loadLightsFromTable(const std::string &identifier, LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        const int tableSize = luaHandler.getLength();

        std::vector<core::YLight> vector;
        vector.reserve(tableSize);
        for (int i = 0; i < tableSize; i++) {
            vector.push_back(YLuaHelper::loadLightFromTable(i + 1, luaHandler));
        }

        return vector;
    }

    core::YLight YLuaHelper::loadLightFromTable(const int index, LuaHandler &luaHandler) {
        luaHandler.getTableFromTable(index);
        return loadLight(luaHandler);
    }

    core::YLight YLuaHelper::loadLightFromTable(const std::string &identifier, LuaHandler &luaHandler) {
        if (luaHandler.isTopOfStackATable()) {
            luaHandler.getTableFromTable(identifier);
        } else {
            luaHandler.loadTable(identifier);
        }
        return loadLight(luaHandler);
    }

    core::YLight YLuaHelper::loadLight(LuaHandler &luaHandler) {
        glm::vec3 lightPositionValue;
        glm::vec3 lightDirectionValue;
        glm::vec3 lightUpValue;
        glm::vec4 lightColorValue;
        float lightIntensityValue;
        float lightSpecularValue;
        bool lightDirectionalValue;

        if (luaHandler.getTableFromTable("pos")) {
            lightPositionValue.x = luaHandler.getNumberFromTable(1);
            lightPositionValue.y = luaHandler.getNumberFromTable(2);
            lightPositionValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("dir")) {
            lightDirectionValue.x = luaHandler.getNumberFromTable(1);
            lightDirectionValue.y = luaHandler.getNumberFromTable(2);
            lightDirectionValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("up")) {
            lightUpValue.x = luaHandler.getNumberFromTable(1);
            lightUpValue.y = luaHandler.getNumberFromTable(2);
            lightUpValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("col")) {
            lightColorValue.x = luaHandler.getNumberFromTable(1);
            lightColorValue.y = luaHandler.getNumberFromTable(2);
            lightColorValue.z = luaHandler.getNumberFromTable(3);
            lightColorValue.w = luaHandler.getNumberFromTable(4);
            luaHandler.popTable();
        }

        lightIntensityValue = luaHandler.getNumberFromTable("intensity");
        lightSpecularValue = luaHandler.getNumberFromTable("specularPower");
        lightDirectionalValue = luaHandler.getBoolFromTable("directional");
        luaHandler.popTable();

        core::YLight light(lightPositionValue, lightDirectionValue, lightColorValue, lightIntensityValue,
                           lightDirectionalValue);
        light.setUp(lightUpValue);
        light.setSpecularPower(lightSpecularValue);
        return light;
    }

    core::YLight &
    YLuaHelper::updateLight(const LuaHandler &luaHandler, core::YLight &light, const std::string &updateFunction,
                            const float deltaTime) {
        if (luaHandler.getFunction(updateFunction)) {
            luaHandler.pushNumber(deltaTime);
            glm::vec3 position = light.getPosition();
            luaHandler.pushNumber(position.x);
            luaHandler.pushNumber(position.y);
            luaHandler.pushNumber(position.z);
            glm::vec3 direction = light.getDirection();
            luaHandler.pushNumber(direction.x);
            luaHandler.pushNumber(direction.y);
            luaHandler.pushNumber(direction.z);
            glm::vec3 up = light.getUp();
            luaHandler.pushNumber(up.x);
            luaHandler.pushNumber(up.y);
            luaHandler.pushNumber(up.z);
            glm::vec4 color = light.getColor();
            luaHandler.pushNumber(color.x);
            luaHandler.pushNumber(color.y);
            luaHandler.pushNumber(color.z);
            luaHandler.pushNumber(color.w);
            luaHandler.pushNumber(light.getIntensity());

            luaHandler.callFunctionFromStack(15, 14);

            position.x = luaHandler.popNumber();
            position.y = luaHandler.popNumber();
            position.z = luaHandler.popNumber();
            light.setPosition(position);
            direction.x = luaHandler.popNumber();
            direction.y = luaHandler.popNumber();
            direction.z = luaHandler.popNumber();
            light.setDirection(direction);
            up.x = luaHandler.popNumber();
            up.y = luaHandler.popNumber();
            up.z = luaHandler.popNumber();
            light.setUp(up);
            color.x = luaHandler.popNumber();
            color.y = luaHandler.popNumber();
            color.z = luaHandler.popNumber();
            color.w = luaHandler.popNumber();
            light.setColor(color);
            light.setIntensity(luaHandler.popNumber());
        }

        return light;
    }

    void YLuaHelper::setupCameraPosition(const std::string &cameraTable, std::shared_ptr<core::YCamera> camera,
                                         const LuaHandler &luaHandler) {
        luaHandler.loadTable(cameraTable);
        glm::vec3 positionValue(0.0f, 0.0f, 0.0f);
        glm::vec3 directionValue(0.0f, 0.0f, 1.0f);
        glm::vec3 upValue(0.0f, 1.0f, 0.0f);
        glm::vec3 rightValue(1.0f, 0.0f, 0.0f);
        glm::vec2 mouseAngle(0.0f, 0.0f);

        if (luaHandler.getTableFromTable("pos")) {
            positionValue.x = luaHandler.getNumberFromTable(1);
            positionValue.y = luaHandler.getNumberFromTable(2);
            positionValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("up")) {
            upValue.x = luaHandler.getNumberFromTable(1);
            upValue.y = luaHandler.getNumberFromTable(2);
            upValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("right")) {
            rightValue.x = luaHandler.getNumberFromTable(1);
            rightValue.y = luaHandler.getNumberFromTable(2);
            rightValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }

        if (luaHandler.getTableFromTable("dir")) {
            directionValue.x = luaHandler.getNumberFromTable(1);
            directionValue.y = luaHandler.getNumberFromTable(2);
            directionValue.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }
        if (luaHandler.getTableFromTable("angle")) {
            mouseAngle.x = luaHandler.getNumberFromTable(1);
            mouseAngle.y = luaHandler.getNumberFromTable(2);
            luaHandler.popTable();
            camera->setMouseAngle(mouseAngle);
        } else {
            camera->CalculateRotationFromDirection(directionValue);
        }
        luaHandler.popTable();

        camera->setPos(positionValue);
        camera->setDir(directionValue);
        camera->setRight(rightValue);
        camera->setUp(upValue);
    }

    core::YAmbientLight
    YLuaHelper::loadAmbientLightFromTable(const std::string &identifier, const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        glm::vec4 lightColorValue(0.0f, 0.0f, 0.0f, 0.0f);

        if (luaHandler.getTableFromTable("col")) {
            lightColorValue.x = luaHandler.getNumberFromTable(1);
            lightColorValue.y = luaHandler.getNumberFromTable(2);
            lightColorValue.z = luaHandler.getNumberFromTable(3);
            lightColorValue.w = luaHandler.getNumberFromTable(4);
            luaHandler.popTable();
        }

        const float lightIntensityValue = luaHandler.getNumberFromTable("intensity");
        luaHandler.popTable();

        return {lightColorValue, lightIntensityValue};
    }

    elements::YFog YLuaHelper::loadFogFromTable(const std::string &identifier, const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        const float maxDist = luaHandler.getNumberFromTable("maxDist");
        const float minDist = luaHandler.getNumberFromTable("minDist");

        glm::vec4 color;
        luaHandler.getTableFromTable("color");
        color.x = luaHandler.getNumberFromTable(1);
        color.y = luaHandler.getNumberFromTable(2);
        color.z = luaHandler.getNumberFromTable(3);
        color.w = luaHandler.getNumberFromTable(4);
        luaHandler.popTable();
        luaHandler.popTable();

        return {maxDist, minDist, color};
    }

    glm::vec4 YLuaHelper::readVec4FromTable(const std::string &identifier, const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        glm::vec4 value(0, 0, 0, 0);
        value.x = luaHandler.getNumberFromTable(1);
        value.y = luaHandler.getNumberFromTable(2);
        value.z = luaHandler.getNumberFromTable(3);
        value.w = luaHandler.getNumberFromTable(4);
        luaHandler.popTable();
        return value;
    }

    glm::vec3 YLuaHelper::readVec3FromTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::vec3 value(0, 0, 0);
        if(luaHandler.loadTable(identifier)) {
            value.x = luaHandler.getNumberFromTable(1);
            value.y = luaHandler.getNumberFromTable(2);
            value.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }
        return value;
    }

    glm::vec3 YLuaHelper::readVec3FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::vec3 vec3Value(0.0f, 0.0f, 0.0f);
        if (luaHandler.getTableFromTable(identifier)) {
            vec3Value.x = luaHandler.getNumberFromTable(1);
            vec3Value.y = luaHandler.getNumberFromTable(2);
            vec3Value.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }
        return vec3Value;
    }

    std::vector<std::string>
    YLuaHelper::readListOfStringsFromTable(const std::string &identifier, const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        auto l = luaHandler.getLength();
        std::vector<std::string> result;

        for (int i = 1; i <= l; i++) {
            auto string = luaHandler.getStringFromTable(i);
            result.push_back(string);
        }

        return result;
    }

    glm::vec2 YLuaHelper::readVec2FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::vec2 vec2Value(0.0f, 0.0f);
        if (luaHandler.getTableFromTable(identifier)) {
            vec2Value.x = luaHandler.getNumberFromTable(1);
            vec2Value.y = luaHandler.getNumberFromTable(2);
            luaHandler.popTable();
        }
        return vec2Value;
    }

    glm::vec4 YLuaHelper::readVec4FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::vec4 vec4Value(0.0f, 0.0f, 0.0f, 0.0f);
        if (luaHandler.getTableFromTable(identifier)) {
            vec4Value.x = luaHandler.getNumberFromTable(1);
            vec4Value.y = luaHandler.getNumberFromTable(2);
            vec4Value.z = luaHandler.getNumberFromTable(3);
            vec4Value.w = luaHandler.getNumberFromTable(4);
            luaHandler.popTable();
        }
        return vec4Value;
    }

    glm::mat3 YLuaHelper::readMat3FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::mat3 mat3Value(0.0f);
        if (luaHandler.getTableFromTable(identifier)) {
            for (int i = 1, k = 0; i < 10; i += 3) {
                glm::vec3 vec3Value;
                vec3Value.x = luaHandler.getNumberFromTable(i);
                vec3Value.y = luaHandler.getNumberFromTable(i + 1);
                vec3Value.z = luaHandler.getNumberFromTable(i + 2);
                mat3Value[k++] = vec3Value;
            }
            luaHandler.popTable();
        }
        return mat3Value;
    }

    glm::mat4 YLuaHelper::readMat4FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::mat4 mat4Value(0.0f);
        if (luaHandler.getTableFromTable(identifier)) {
            for (int i = 1, k = 0; i < 17; i += 4) {
                glm::vec4 vec4Value;
                vec4Value.x = luaHandler.getNumberFromTable(i);
                vec4Value.y = luaHandler.getNumberFromTable(i + 1);
                vec4Value.z = luaHandler.getNumberFromTable(i + 2);
                vec4Value.w = luaHandler.getNumberFromTable(i + 3);
                mat4Value[k++] = vec4Value;
            }
            luaHandler.popTable();
        }
        return mat4Value;
    }
} // utils
