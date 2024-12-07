//
// Created by Yvens Serpa on 07/12/2024.
//

#pragma once
#include <glm/glm.hpp>

namespace core {
    class YCamera {
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection;
        glm::vec3 up;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;

        float horizontalAngle;
        float verticalAngle;
        float zoom;
        float near;
        float far;

    public:
        YCamera();

        YCamera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up);

        ~YCamera();

        void MoveForward(float step);

        void MoveSideways(float step);

        void MoveUp(float step);

        void RotateWithMouse(float horizontalAngle, float verticalAngle);

        void CalculateRotationFromDirection(const glm::vec3 direction);

        void Zoom(float zoom);

        glm::mat4 getView() const;

        glm::vec3 getPos() const;

        glm::vec3 getDir() const;

        glm::vec3 getUp() const;

        glm::vec3 getRight() const;

        float getZoom() const;

        float getNear() const;

        float getFar() const;

        glm::vec2 getAngles() const;

        void setPos(const glm::vec3 value);

        void setDir(const glm::vec3 value);

        void setUp(const glm::vec3 value);

        void setRight(const glm::vec3 value);

        void setMouseAngle(const glm::vec2 value);
    };
} // core
