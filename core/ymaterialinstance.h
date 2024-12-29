//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include "ymaterial.h"
#include "../LuaHandler/luahandler.hpp"

namespace core {
    class YModel;

    struct YBaseUniformValue;

    class YMaterialInstance {
        GLuint modelUniformLocation;
        GLuint viewProjectionUniformLocation;
        GLuint numberPointLightsLocation;
        GLuint numberDirectionLightsLocation;
        int numberPointLights;
        int numberDirectionLights;
        std::shared_ptr<core::YMaterial> material;
        std::vector<std::shared_ptr<YBaseUniformValue> > uniformValues;

    public:
        explicit YMaterialInstance(const std::shared_ptr<YMaterial> &material, const std::string &materialTableName,
                                   const LuaHandler &luaHandler);

        void drawModel(const glm::mat4 &modelMatrix, const std::shared_ptr<YModel> &model,
                       const glm::mat4 &viewProjection) const;

        void drawModel(const std::shared_ptr<YModel> &model, const glm::mat4 &viewProjection) const;

        void addUniformValue(const std::shared_ptr<YBaseUniformValue>& uniformValue);

        bool doesSupportLight() const;

        void updateNumberOfLights(int numberPointLights, int numberDirectionalLights);
    };
} // core
