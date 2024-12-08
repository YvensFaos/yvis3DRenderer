//
// Created by Yvens Serpa on 08/12/2024.
//

#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace core {
    class YModel;
    class YInstanceMesh;
    class YCamera;
}

class LuaHandler;

// class YLight;
// class YAmbientLight;

// class YFog;

namespace utils {
    class YLuaHelper {
    public:
        ///Load a vector of AModel pointers from a table with given identifier. Pop the table at the end.
        static std::vector<core::YModel> loadModelsFromTable(const std::string &identifier, const LuaHandler &luaHandler);

        ///Load a AModel from a table, popping it out of the stack after it finishes reading it.
        static core::YModel loadModelFromTable(const std::string &identifier, const LuaHandler &luaHandler);

        ///Load a AModel from a table on top of the stack. Optionally pop the table at the end.
        static core::YModel loadModel(const LuaHandler &luaHandler, bool popTable = false);

        //Load a AInstanceMesh from a table. Optionally pop the table at the end.
        static core::YInstanceMesh loadInstanceMeshFromTable(const std::string& identifier, const LuaHandler &luaHandler,
                                                       bool popTable = false);

        /////Load a list of pointers of ALight from a table, popping it out of the stack after it finishes reading it.
        //static std::vector<ALight> loadLightsFromTable(std::string identifier, LuaHandler& luaHandler);
        /////Load a ALight from a table, popping it out of the stack after it finishes reading it.
        //static ALight loadLightFromTable(int index, LuaHandler&luaHandler);
        /////Load a ALight from a table, popping it out of the stack after it finishes reading it.
        //static ALight loadLightFromTable(std::string identifier, LuaHandler& luaHandler);
        /////Load a ALight from a table on top of the stack. Optionally pop the table at the end.
        //static ALight loadLight(LuaHandler& luaHandler, bool popTable = false);
        /////Update a ALight properties using a function named [updateFunction] accessed by [luaHandler]
        //static ALight& updateLight(LuaHandler& luaHandler, ALight& alight, std::string updateFunction, float deltaTime);

        ///Load camera position, up and right parameters from the cameraTable and set it to the acamera pointer.
        static void setupCameraPosition(const std::string &cameraTable, core::YCamera &camera, const LuaHandler &luaHandler);

        ///Load a AAmbientLight from a table, popping it out of the stack after it finises reading it.
        // static AAmbientLight loadAmbientLightFromTable(std::string identifier, LuaHandler& luaHandler);

        ///Load a AFog from a table, popping it out of the stack after it finishes reading it.
        // static AFog loadFogFromTable(std::string identifier, LuaHandler& luaHandler);

        ///Load a glm::vec4 from 4 consecutive values in a table by the identifier name.
        static glm::vec4 readVec4FromTable(const std::string &identifier, const LuaHandler &luaHandler);

        ///Load a glm::vec3 from 3 consecutive values in a table by the identifier name, when the table is in global scope.
        static glm::vec3 readVec3FromTable(const std::string &identifier, const LuaHandler &luaHandler);

        ///Load a glm::vec3 from 3 consecutive values in a table by the identifier name, when inside a table.
        static glm::vec3 readVec3FromTableInTable(const std::string &identifier, const LuaHandler &luaHandler);
    };
} // utils
