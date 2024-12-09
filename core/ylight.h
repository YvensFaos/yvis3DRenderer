//
// Created by Yvens Serpa on 09/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace core {
    class YAmbientLight {
        glm::vec4 color;
        float intensity;

    public:
        YAmbientLight(glm::vec4 color, float intensity);

        ~YAmbientLight() = default;

        [[nodiscard]] glm::vec4 getColor() const;

        [[nodiscard]] float getIntensity() const;

        void setColor(glm::vec4 color);

        void setIntensity(float intensity);

        void setupUniforms(GLuint ambientLightColorUniform, GLuint ambientLightIntensityUniform) const;
    };

    struct YLightUniform;

    class YLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;
        glm::vec4 color;
        float intensity;
        float specularPower{};
        bool directional;

    public:
        YLight(glm::vec3 position, glm::vec3 direction, glm::vec4 color, float intensity, bool directional = true);

        YLight(const YLight &anotherLight);

        ~YLight() = default;

        [[nodiscard]] glm::vec3 getPosition() const;

        [[nodiscard]] glm::vec3 getDirection() const;

        [[nodiscard]] glm::vec3 getUp() const;

        [[nodiscard]] glm::vec4 getColor() const;

        [[nodiscard]] float getIntensity() const;

        [[nodiscard]] float getSpecularPower() const;

        [[nodiscard]] bool getDirectional() const;

        void setPosition(glm::vec3 position);

        void setDirection(glm::vec3 direction);

        void setUp(glm::vec3 up);

        void setColor(glm::vec4 color);

        void setIntensity(float intensity);

        void setSpecularPower(float specularPower);

        void setupUniforms(GLuint lightPositionUniform, GLuint lightDirectionUniform, GLuint lightColorUniform,
                           GLuint lightIntensityUniform, GLuint lightDirectionalUniform,
                           GLuint lightSpecularUniform = -1) const;

        void setupUniforms(const YLightUniform &lightUniform) const;

        YLight &operator=(const YLight &anotherLight);

        void log() const;
    };

    struct YLightUniform {
        GLuint lightPositionUniform;
        GLuint lightDirectionUniform;
        GLuint lightColorUniform;
        GLuint lightIntensityUniform;
        GLuint lightSpecularUniform;
        GLuint lightDirectionalUniform;

        YLightUniform(const GLuint lightPositionUniform, const GLuint lightDirectionUniform, const GLuint lightColorUniform,
                      const GLuint lightIntensityUniform, const GLuint lightDirectionalUniform,
                      const GLuint lightSpecularUniform = -1) : lightPositionUniform(lightPositionUniform),
                                                                lightDirectionUniform(lightDirectionUniform),
                                                                lightColorUniform(lightColorUniform),
                                                                lightIntensityUniform(lightIntensityUniform),
                                                                lightSpecularUniform(lightSpecularUniform),
                                                                lightDirectionalUniform(lightDirectionalUniform) {
        }

        YLightUniform(const YLightUniform &copyFrom) {
            this->lightPositionUniform = copyFrom.lightPositionUniform;
            this->lightDirectionUniform = copyFrom.lightDirectionUniform;
            this->lightColorUniform = copyFrom.lightColorUniform;
            this->lightIntensityUniform = copyFrom.lightIntensityUniform;
            this->lightDirectionalUniform = copyFrom.lightDirectionalUniform;
            this->lightSpecularUniform = copyFrom.lightSpecularUniform;
        }

        ~YLightUniform() = default;

        YLightUniform &operator=(const YLightUniform &copyFrom) {
            if (this != &copyFrom) {
                this->lightPositionUniform = copyFrom.lightPositionUniform;
                this->lightDirectionUniform = copyFrom.lightDirectionUniform;
                this->lightColorUniform = copyFrom.lightColorUniform;
                this->lightIntensityUniform = copyFrom.lightIntensityUniform;
                this->lightDirectionalUniform = copyFrom.lightDirectionalUniform;
                this->lightSpecularUniform = copyFrom.lightSpecularUniform;
            }
            return *this;
        }

        static YLightUniform loadYLightUniformFromProgramme(GLuint shaderProgramme, GLuint lightIndex,
                                                            const YLight &YLight);

        static YLightUniform loadYLightUniformFromProgrammeWithName(GLuint shaderProgramme, GLuint lightIndex,
                                                                    const YLight &YLight, const std::string& name);
    };
}