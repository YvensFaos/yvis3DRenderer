//
// Created by Yvens Serpa on 09/12/2024.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace core {
    struct YYLightUniform;

    class YAmbientLight {
        glm::vec4 color;
        float intensity;

    public:
        YAmbientLight(glm::vec4 color, float intensity);

        ~YAmbientLight() = default;

        [[nodiscard]] glm::vec4 getColor() const;

        [[nodiscard]] float getIntensity() const;

        void setColor(glm::vec4 newColor);

        void setIntensity(float newIntensity);

        void setupUniforms(GLuint ambientLightColorUniform, GLuint ambientLightIntensityUniform) const;
    };

    struct YYLightUniform;

    class YLight {
        int index;
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;
        glm::vec4 color;
        float intensity;
        float specularPower;
        bool directional;

    public:
        YLight(glm::vec3 position, glm::vec3 direction, glm::vec4 color, float intensity, bool directional = true);

        YLight(const YLight &anotherLight);

        ~YLight() = default;

        [[nodiscard]] int getIndex() const;

        [[nodiscard]] glm::vec3 getPosition() const;

        [[nodiscard]] glm::vec3 getDirection() const;

        [[nodiscard]] glm::vec3 getUp() const;

        [[nodiscard]] glm::vec4 getColor() const;

        [[nodiscard]] float getIntensity() const;

        [[nodiscard]] float getSpecularPower() const;

        [[nodiscard]] bool getDirectional() const;

        void setIndex(int index);

        void setPosition(glm::vec3 newPosition);

        void setDirection(glm::vec3 newDirection);

        void setUp(glm::vec3 up);

        void setColor(glm::vec4 newColor);

        void setIntensity(float newIntensity);

        void setSpecularPower(float newSpecularPower);

        void setupUniforms(GLuint lightPositionUniform, GLuint lightDirectionUniform, GLuint lightColorUniform,
                           GLuint lightIntensityUniform, GLuint lightDirectionalUniform,
                           GLuint lightSpecularUniform = -1) const;

        void setupUniforms(const YYLightUniform &lightUniform) const;

        YLight &operator=(const YLight &anotherLight);

        void log() const;
    };

    //Deprecated
    struct YYLightUniform {
        GLuint lightPositionUniform;
        GLuint lightDirectionUniform;
        GLuint lightColorUniform;
        GLuint lightIntensityUniform;
        GLuint lightSpecularUniform;
        GLuint lightDirectionalUniform;

        YYLightUniform(const GLuint lightPositionUniform, const GLuint lightDirectionUniform, const GLuint lightColorUniform,
                      const GLuint lightIntensityUniform, const GLuint lightDirectionalUniform,
                      const GLuint lightSpecularUniform = -1) : lightPositionUniform(lightPositionUniform),
                                                                lightDirectionUniform(lightDirectionUniform),
                                                                lightColorUniform(lightColorUniform),
                                                                lightIntensityUniform(lightIntensityUniform),
                                                                lightSpecularUniform(lightSpecularUniform),
                                                                lightDirectionalUniform(lightDirectionalUniform) {
        }

        YYLightUniform(const YYLightUniform &copyFrom) {
            this->lightPositionUniform = copyFrom.lightPositionUniform;
            this->lightDirectionUniform = copyFrom.lightDirectionUniform;
            this->lightColorUniform = copyFrom.lightColorUniform;
            this->lightIntensityUniform = copyFrom.lightIntensityUniform;
            this->lightDirectionalUniform = copyFrom.lightDirectionalUniform;
            this->lightSpecularUniform = copyFrom.lightSpecularUniform;
        }

        ~YYLightUniform() = default;

        YYLightUniform &operator=(const YYLightUniform &copyFrom) {
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

        static YYLightUniform loadYLightUniformFromProgramme(GLuint shaderProgramme, GLuint lightIndex,
                                                            const YLight &YLight);

        static YYLightUniform loadYLightUniformFromProgrammeWithName(GLuint shaderProgramme, GLuint lightIndex,
                                                                    const YLight &YLight, const std::string& name);
    };
}