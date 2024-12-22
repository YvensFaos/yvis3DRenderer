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
    private:
        GLuint modelUniformLocation;
        GLuint viewProjectionUniformLocation;
        std::shared_ptr<core::YMaterial> material;
        std::vector<std::shared_ptr<YBaseUniformValue>> uniformValues;
    public:
        explicit YMaterialInstance(std::shared_ptr<YMaterial>& material, const std::string& materialTableName, LuaHandler& luaHandler);

        void drawModel(const glm::mat4& modelMatrix, const std::shared_ptr<YModel>& model, const glm::mat4& viewProjection) const;
        void drawModel(const std::shared_ptr<YModel>& model, const glm::mat4& viewProjection) const;
    };

} // core

