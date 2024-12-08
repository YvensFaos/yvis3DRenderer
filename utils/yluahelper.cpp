//
// Created by Yvens Serpa on 08/12/2024.
//

#include "yluahelper.h"
#include "../core/ymodel.h"
// #include "../Core/alight.hpp"
#include "../core/ycamera.h"
// #include "../Objects/afog.hpp"
#include "../LuaHandler/luahandler.hpp"

namespace utils {
    std::vector<core::YModel> YLuaHelper::loadModelsFromTable(const std::string &identifier,
                                                              const LuaHandler &luaHandler) {
        luaHandler.loadTable(identifier);
        const GLuint tableSize = luaHandler.getLength();
        std::vector<core::YModel> models;

        for (int i = 1; i <= tableSize; i++) {
            if(luaHandler.getTableFromTable(i)) {
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

        if(luaHandler.getTableFromTable("pos")) {
            glm::vec3 translateTo;
            translateTo.x = luaHandler.getNumberFromTable(1);
            translateTo.y = luaHandler.getNumberFromTable(2);
            translateTo.z = luaHandler.getNumberFromTable(3);
            model.translate(translateTo);
            luaHandler.popTable();
        }


        if(luaHandler.getTableFromTable("rot")) {
            glm::vec3 rotateTo;
            rotateTo.x = luaHandler.getNumberFromTable(1);
            rotateTo.y = luaHandler.getNumberFromTable(2);
            rotateTo.z = luaHandler.getNumberFromTable(3);
            model.rotate(rotateTo);
            luaHandler.popTable();
        }

        if(luaHandler.getTableFromTable("sca")) {
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

    core::YInstanceMesh YLuaHelper::loadInstanceMeshFromTable(const std::string& identifier, const LuaHandler &luaHandler,
                                                              const bool popTable) {
        luaHandler.loadTable(identifier);

        const core::YModel model(luaHandler.getStringFromTable("file"));
        const core::YMesh& singleMesh = model.getMeshAt(0);

        std::vector<glm::mat4> instanceData;
        if(luaHandler.getTableFromTable("instanceData")) {
            const GLuint tableSize = luaHandler.getLength();

            for (int i = 1; i <= tableSize; i++) {
                if(luaHandler.getTableFromTable(i)) {
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

    // std::vector<ALight> YLuaHelper::loadLightsFromTable(std::string identifier, LuaHandler &luaHandler) {
    //     luaHandler.loadTable(identifier);
    //     const int tableSize = luaHandler.getLength();
    //
    //     std::vector<ALight> vector;
    //     vector.reserve(tableSize);
    //     for (unsigned int i = 0; i < tableSize; i++) {
    //         vector.push_back(YLuaHelper::loadLightFromTable(i + 1, luaHandler));
    //     }
    //
    //     return vector;
    // }

    // ALight YLuaHelper::loadLightFromTable(int index, LuaHandler &luaHandler) {
    //     luaHandler.getTableFromTable(index);
    //     return YLuaHelper::loadLight(luaHandler, true);
    // }

    // ALight YLuaHelper::loadLightFromTable(std::string identifier, LuaHandler &luaHandler) {
    //     if (luaHandler.isTopOfStackATable()) {
    //         luaHandler.getTableFromTable(identifier);
    //     } else {
    //         luaHandler.loadTable(identifier);
    //     }
    //     return YLuaHelper::loadLight(luaHandler, true);
    // }

    // ALight YLuaHelper::loadLight(LuaHandler &luaHandler, bool popTable) {
    //     glm::vec3 lightPositionValue;
    //     glm::vec3 lightDirectionValue;
    //     glm::vec3 lightUpValue;
    //     glm::vec4 lightColorValue;
    //     float lightIntensityValue;
    //     float lightSpecularValue;
    //     bool lightDirectionalValue;
    //
    //     luaHandler.getTableFromTable("pos");
    //     lightPositionValue.x = luaHandler.getNumberFromTable(1);
    //     lightPositionValue.y = luaHandler.getNumberFromTable(2);
    //     lightPositionValue.z = luaHandler.getNumberFromTable(3);
    //     luaHandler.popTable();
    //
    //     luaHandler.getTableFromTable("dir");
    //     lightDirectionValue.x = luaHandler.getNumberFromTable(1);
    //     lightDirectionValue.y = luaHandler.getNumberFromTable(2);
    //     lightDirectionValue.z = luaHandler.getNumberFromTable(3);
    //     luaHandler.popTable();
    //
    //     luaHandler.getTableFromTable("up");
    //     lightUpValue.x = luaHandler.getNumberFromTable(1);
    //     lightUpValue.y = luaHandler.getNumberFromTable(2);
    //     lightUpValue.z = luaHandler.getNumberFromTable(3);
    //     luaHandler.popTable();
    //
    //     luaHandler.getTableFromTable("col");
    //     lightColorValue.x = luaHandler.getNumberFromTable(1);
    //     lightColorValue.y = luaHandler.getNumberFromTable(2);
    //     lightColorValue.z = luaHandler.getNumberFromTable(3);
    //     lightColorValue.w = luaHandler.getNumberFromTable(4);
    //     luaHandler.popTable();
    //
    //     lightIntensityValue = luaHandler.getNumberFromTable("intensity");
    //     lightSpecularValue = luaHandler.getNumberFromTable("specularPower");
    //     lightDirectionalValue = luaHandler.getBoolFromTable("directional");
    //     luaHandler.popTable();
    //
    //     ALight alight(lightPositionValue, lightDirectionValue, lightColorValue, lightIntensityValue,
    //                   lightDirectionalValue);
    //     alight.setUp(lightUpValue);
    //     alight.setSpecularPower(lightSpecularValue);
    //     return alight;
    // }

    // ALight &YLuaHelper::updateLight(LuaHandler &luaHandler, ALight &alight, std::string updateFunction,
    //                                 float deltaTime) {
    //     if (luaHandler.getFunction(updateFunction)) {
    //         luaHandler.pushNumber(deltaTime);
    //         glm::vec3 position = alight.getPosition();
    //         luaHandler.pushNumber(position.x);
    //         luaHandler.pushNumber(position.y);
    //         luaHandler.pushNumber(position.z);
    //         glm::vec3 direction = alight.getDirection();
    //         luaHandler.pushNumber(direction.x);
    //         luaHandler.pushNumber(direction.y);
    //         luaHandler.pushNumber(direction.z);
    //         glm::vec3 up = alight.getUp();
    //         luaHandler.pushNumber(up.x);
    //         luaHandler.pushNumber(up.y);
    //         luaHandler.pushNumber(up.z);
    //         glm::vec4 color = alight.getColor();
    //         luaHandler.pushNumber(color.x);
    //         luaHandler.pushNumber(color.y);
    //         luaHandler.pushNumber(color.z);
    //         luaHandler.pushNumber(color.w);
    //         luaHandler.pushNumber(alight.getIntensity());
    //
    //         luaHandler.callFunctionFromStack(15, 14);
    //
    //         position.x = luaHandler.popNumber();
    //         position.y = luaHandler.popNumber();
    //         position.z = luaHandler.popNumber();
    //         alight.setPosition(position);
    //         direction.x = luaHandler.popNumber();
    //         direction.y = luaHandler.popNumber();
    //         direction.z = luaHandler.popNumber();
    //         alight.setDirection(direction);
    //         up.x = luaHandler.popNumber();
    //         up.y = luaHandler.popNumber();
    //         up.z = luaHandler.popNumber();
    //         alight.setUp(up);
    //         color.x = luaHandler.popNumber();
    //         color.y = luaHandler.popNumber();
    //         color.z = luaHandler.popNumber();
    //         color.w = luaHandler.popNumber();
    //         alight.setColor(color);
    //         alight.setIntensity(luaHandler.popNumber());
    //     }
    //
    //     return alight;
    // }

    void YLuaHelper::setupCameraPosition(const std::string &cameraTable, core::YCamera &camera,
                                         const LuaHandler &luaHandler) {
        luaHandler.loadTable(cameraTable);
        glm::vec3 positionValue(0.0f, 0.0f, 0.0f);
        glm::vec3 directionValue(0.0f, 0.0f, 1.0f);
        glm::vec3 upValue(0.0f, 1.0f, 0.0f);
        glm::vec3 rightValue(1.0f, 0.0f, 0.0f);
        glm::vec2 mouseAngle(0.0f, 0.0f);

        if(luaHandler.getTableFromTable("pos")) {
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
            camera.setMouseAngle(mouseAngle);
        } else {
            camera.CalculateRotationFromDirection(directionValue);
        }
        luaHandler.popTable();

        camera.setPos(positionValue);
        camera.setDir(directionValue);
        camera.setRight(rightValue);
        camera.setUp(upValue);
    }

    // AAmbientLight YLuaHelper::loadAmbientLightFromTable(std::string identifier, LuaHandler &luaHandler) {
    //     luaHandler.loadTable(identifier);
    //     glm::vec4 lightColorValue;
    //     float lightIntensityValue;
    //
    //     luaHandler.getTableFromTable("col");
    //     lightColorValue.x = luaHandler.getNumberFromTable(1);
    //     lightColorValue.y = luaHandler.getNumberFromTable(2);
    //     lightColorValue.z = luaHandler.getNumberFromTable(3);
    //     lightColorValue.w = luaHandler.getNumberFromTable(4);
    //     luaHandler.popTable();
    //
    //     lightIntensityValue = luaHandler.getNumberFromTable("intensity");
    //     luaHandler.popTable();
    //
    //     return AAmbientLight(lightColorValue, lightIntensityValue);
    // }

    // AFog YLuaHelper::loadFogFromTable(std::string identifier, LuaHandler &luaHandler) {
    //     luaHandler.loadTable(identifier);
    //     float maxDist = luaHandler.getNumberFromTable("maxDist");
    //     float minDist = luaHandler.getNumberFromTable("minDist");
    //
    //     glm::vec4 color;
    //     luaHandler.getTableFromTable("color");
    //     color.x = luaHandler.getNumberFromTable(1);
    //     color.y = luaHandler.getNumberFromTable(2);
    //     color.z = luaHandler.getNumberFromTable(3);
    //     color.w = luaHandler.getNumberFromTable(4);
    //     luaHandler.popTable();
    //
    //     luaHandler.popTable();
    //
    //     return AFog(maxDist, minDist, color);
    // }

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
        luaHandler.loadTable(identifier);
        glm::vec3 value(0, 0, 0);
        value.x = luaHandler.getNumberFromTable(1);
        value.y = luaHandler.getNumberFromTable(2);
        value.z = luaHandler.getNumberFromTable(3);
        luaHandler.popTable();
        return value;
    }

    glm::vec3 YLuaHelper::readVec3FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler) {
        glm::vec3 vec3Value(0.0f, 0.0f, 0.0f);
        if(luaHandler.getTableFromTable(identifier)) {
            vec3Value.x = luaHandler.getNumberFromTable(1);
            vec3Value.y = luaHandler.getNumberFromTable(2);
            vec3Value.z = luaHandler.getNumberFromTable(3);
            luaHandler.popTable();
        }
        return vec3Value;
    }
} // utils
