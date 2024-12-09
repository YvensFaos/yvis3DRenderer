//
// Created by Yvens Serpa on 09/12/2024.
//

#include "ylight.h"

#include <string>

namespace core {
    YAmbientLight::YAmbientLight(const glm::vec4 color, const float intensity)
        : color(color), intensity(intensity) {
    }

    glm::vec4 YAmbientLight::getColor() const {
        return this->color;
    }

    float YAmbientLight::getIntensity() const {
        return this->intensity;
    }

    void YAmbientLight::setColor(const glm::vec4 color) {
        this->color.x = color.x;
        this->color.y = color.y;
        this->color.z = color.z;
        this->color.w = color.w;
    }

    void YAmbientLight::setIntensity(const float intensity) {
        this->intensity = intensity;
    }

    void YAmbientLight::setupUniforms(const GLuint ambientLightColorUniform,
                                      const GLuint ambientLightIntensityUniform) const {
        glUniform4f(static_cast<GLint>(ambientLightColorUniform), this->color.x, this->color.y, this->color.z,
                    this->color.w);
        glUniform1f(static_cast<GLint>(ambientLightIntensityUniform), this->intensity);
    }

    //##### A LIGHT UNIFORM

    YLightUniform YLightUniform::loadYLightUniformFromProgramme(const GLuint shaderProgramme, const GLuint lightIndex,
                                                                const YLight &YLight) {
        return loadYLightUniformFromProgrammeWithName(shaderProgramme, lightIndex, YLight,
                                                      YLight.getDirectional()
                                                          ? "directionalLights"
                                                          : "pointLights");
    }


    YLightUniform YLightUniform::loadYLightUniformFromProgrammeWithName(const GLuint shaderProgramme,
                                                                        const GLuint lightIndex,
                                                                        const YLight &YLight, const std::string &name) {
        GLuint lightPositionUniform = -1;
        GLuint lightDirectionUniform = -1;
        GLuint lightColorUniform = -1;
        GLuint lightIntensityUniform = -1;
        GLuint lightDirectionalUniform = -1;
        GLuint lightSpecularUniform = -1;

        char uniformName[64];
        snprintf(uniformName, 64, "%s[%d].", name.c_str(), lightIndex);
        std::string buffer;

        auto getLightUniformLocation = [](std::string &stringBuffer, char uniName[64], GLuint &uniform,
                                          const GLuint shader, const std::string &uniformProperty) {
            stringBuffer = "";
            stringBuffer.append(uniName);
            uniform = glGetUniformLocation(shader, stringBuffer.append(uniformProperty).c_str());
        };

        getLightUniformLocation(buffer, uniformName, lightPositionUniform, shaderProgramme, "position");
        getLightUniformLocation(buffer, uniformName, lightDirectionUniform, shaderProgramme, "direction");
        getLightUniformLocation(buffer, uniformName, lightColorUniform, shaderProgramme, "color");
        getLightUniformLocation(buffer, uniformName, lightIntensityUniform, shaderProgramme, "intensity");
        getLightUniformLocation(buffer, uniformName, lightDirectionalUniform, shaderProgramme, "directional");
        getLightUniformLocation(buffer, uniformName, lightSpecularUniform, shaderProgramme, "specularPower");

        return {
            lightPositionUniform, lightDirectionUniform, lightColorUniform, lightIntensityUniform,
            lightDirectionalUniform, lightSpecularUniform
        };
    }

    //##### A LIGHT

    YLight::YLight(const glm::vec3 position, const glm::vec3 direction, const glm::vec4 color, const float intensity,
                   const bool directional)
        : position(position), direction(direction), up(glm::vec3(0, 1, 0)), color(color), intensity(intensity),
          directional(directional) {
    }

    YLight::YLight(const YLight &anotherLight)
        : position(anotherLight.getPosition()), direction(anotherLight.getDirection()), up(anotherLight.getUp()),
          color(anotherLight.getColor()),
          intensity(anotherLight.getIntensity()), specularPower(anotherLight.getSpecularPower()),
          directional(anotherLight.getDirectional()) {
    }

    void YLight::setupUniforms(const GLuint lightPositionUniform, const GLuint lightDirectionUniform,
                               const GLuint lightColorUniform,
                               const GLuint lightIntensityUniform, const GLuint lightDirectionalUniform,
                               const GLuint lightSpecularUniform) const {
        glUniform3f(static_cast<GLint>(lightPositionUniform), position.x, position.y, position.z);
        glUniform3f(static_cast<GLint>(lightDirectionUniform), direction.x, direction.y, direction.z);
        glUniform4f(static_cast<GLint>(lightColorUniform), color.x, color.y, color.z, color.w);
        glUniform1f(static_cast<GLint>(lightIntensityUniform), intensity);
        glUniform1i(static_cast<GLint>(lightDirectionalUniform), directional);
        glUniform1f(static_cast<GLint>(lightSpecularUniform), specularPower);
    }

    void YLight::setupUniforms(const YLightUniform &lightUniform) const {
        glUniform3f(static_cast<GLint>(lightUniform.lightPositionUniform), position.x, position.y, position.z);
        glUniform3f(static_cast<GLint>(lightUniform.lightDirectionUniform), direction.x, direction.y, direction.z);
        glUniform4f(static_cast<GLint>(lightUniform.lightColorUniform), color.x, color.y, color.z, color.w);
        glUniform1f(static_cast<GLint>(lightUniform.lightIntensityUniform), intensity);
        glUniform1i(static_cast<GLint>(lightUniform.lightDirectionalUniform), directional);
        glUniform1f(static_cast<GLint>(lightUniform.lightSpecularUniform), specularPower);
    }

    glm::vec3 YLight::getPosition() const {
        return this->position;
    }

    glm::vec3 YLight::getDirection() const {
        return this->direction;
    }

    glm::vec3 YLight::getUp() const {
        return this->up;
    }

    glm::vec4 YLight::getColor() const {
        return this->color;
    }

    float YLight::getIntensity() const {
        return this->intensity;
    }

    bool YLight::getDirectional() const {
        return this->directional;
    }

    float YLight::getSpecularPower() const {
        return this->specularPower;
    }

    void YLight::setPosition(const glm::vec3 position) {
        this->position.x = position.x;
        this->position.y = position.y;
        this->position.z = position.z;
    }

    void YLight::setDirection(const glm::vec3 direction) {
        this->direction.x = direction.x;
        this->direction.y = direction.y;
        this->direction.z = direction.z;
    }

    void YLight::setColor(const glm::vec4 color) {
        this->color.x = color.x;
        this->color.y = color.y;
        this->color.z = color.z;
        this->color.w = color.w;
    }

    void YLight::setIntensity(const float intensity) {
        this->intensity = intensity;
    }

    void YLight::setSpecularPower(const float specularPower) {
        this->specularPower = specularPower;
    }

    void YLight::setUp(const glm::vec3 up) {
        this->up.x = up.x;
        this->up.y = up.y;
        this->up.z = up.z;
    }

    YLight &YLight::operator=(const YLight &anotherLight) {
        const glm::vec3 position = anotherLight.getPosition();
        this->position.x = position.x;
        this->position.y = position.y;
        this->position.z = position.z;

        const glm::vec3 direction = anotherLight.getDirection();
        this->direction.x = direction.x;
        this->direction.y = direction.y;
        this->direction.z = direction.z;

        const glm::vec3 up = anotherLight.getUp();
        this->up.x = up.x;
        this->up.y = up.y;
        this->up.z = up.z;

        const glm::vec4 color = anotherLight.getColor();
        this->color.x = color.x;
        this->color.y = color.y;
        this->color.z = color.z;
        this->color.w = color.w;

        this->setIntensity(anotherLight.getIntensity());
        this->setSpecularPower(anotherLight.getSpecularPower());
        this->directional = anotherLight.getDirectional();

        return *this;
    }

    void YLight::log() const {
        printf("Light Info: \n");
        printf("---------------\n");
        printf("pos   = {%3.3f, %3.3f, %3.3f},\n", this->position.x, this->position.y, this->position.z);
        printf("dir   = {%3.3f, %3.3f, %3.3f},\n", this->direction.x, this->direction.y, this->direction.z);
        printf("up    = {%3.3f, %3.3f, %3.3f},\n", this->up.x, this->up.y, this->up.z);
        printf("color = {%3.3f, %3.3f, %3.3f, %3.3f},\n", this->color.x, this->color.y, this->color.z, this->color.w);
        printf("inten = {%3.3f}\n", this->getIntensity());
        printf("specp = {%3.3f}\n", this->getSpecularPower());
        printf("direc = {%s}\n", this->directional ? "yes" : "no");
        printf("---------------\n\n");
    }
} // core
