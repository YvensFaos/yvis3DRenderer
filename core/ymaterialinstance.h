//
// Created by Yvens Rebouças Serpa on 23/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <vector>

#include "../LuaHandler/luahandler.hpp"

namespace core {
    class YModel;

    class YMaterial;

    struct YBaseUniformValue;
    struct YUniform;

    class YMaterialInstance {
        GLuint modelUniformLocation;
        GLuint viewProjectionUniformLocation;
        GLuint numberPointLightsLocation;
        GLuint numberDirectionLightsLocation;
        int numberPointLights;
        int numberDirectionLights;
        std::shared_ptr<core::YMaterial> material;
        std::vector<std::shared_ptr<core::YBaseUniformValue> > uniformValues;

    public:
        explicit YMaterialInstance(const std::shared_ptr<YMaterial> &material);

        explicit YMaterialInstance(const std::shared_ptr<YMaterial> &material, const std::string &materialTableName,
                                   const LuaHandler &luaHandler);

        void drawModel(const glm::mat4 &modelMatrix, const std::shared_ptr<YModel> &model,
                       const glm::mat4 &viewProjection) const;

        void drawModel(const std::shared_ptr<YModel> &model, const glm::mat4 &viewProjection) const;

        void addUniformValue(const std::shared_ptr<YBaseUniformValue> &uniformValue);

        template<typename T>
        void updateUniformValue(const std::string &baseUniformValue, T uniformValue);

        [[nodiscard]] bool doesSupportLight() const;

        void updateNumberOfLights(int pointLights, int numberDirectionalLights);

        std::shared_ptr<core::YMaterial> &getMaterial();

    private:
        bool getModelUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                     std::vector<std::shared_ptr<core::YUniform> >::const_iterator &uniformsIterator,
                                     const std::string &identifier);

        bool getViewProjectionUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                              std::vector<std::shared_ptr<core::YUniform> >::const_iterator &
                                              uniformsIterator,
                                              const std::string &identifier);

        bool getNumberPointLightsUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                 std::vector<std::shared_ptr<core::YUniform> >::const_iterator &
                                                 uniformsIterator,
                                                 const std::string &identifier);

        bool getNumberDirectionLightsUniformLocation(const std::shared_ptr<YMaterial> &yMaterial,
                                                     std::vector<std::shared_ptr<core::YUniform> >::const_iterator &
                                                     uniformsIterator, const std::string &identifier);

        std::shared_ptr<YBaseUniformValue> findUniform(const std::string &uniformName);
    };
} // core
