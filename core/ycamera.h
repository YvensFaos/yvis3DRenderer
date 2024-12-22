//
// Created by Yvens Serpa on 07/12/2024.
//

#pragma once
#include <glm/glm.hpp>

namespace core {
    class YCamera {
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraDirection{};
        glm::vec3 up;
        glm::vec3 cameraRight{};
        glm::vec3 cameraUp{};

        float horizontalAngle;
        float verticalAngle;
        float zoom;
        float near;
        float far;

        glm::mat4 cachedViewProjection;

    public:
        YCamera();

        YCamera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up);

        ~YCamera();

        void MoveForward(float step);

        void MoveSideways(float step);

        void MoveUp(float step);

        void RotateWithMouse(float hAngle, float vAngle);

        void CalculateRotationFromDirection(glm::vec3 direction);

        void Zoom(float newZoom);

        void cacheViewProjectionMatrix(float width, float height);

        [[nodiscard]] glm::mat4 getCachedViewProjectionMatrix() const;

        [[nodiscard]] glm::mat4 getViewProjectionMatrix(float width, float height) const;

        [[nodiscard]] glm::mat4 getView() const;

        [[nodiscard]] glm::vec3 getPos() const;

        [[nodiscard]] glm::vec3 getDir() const;

        [[nodiscard]] glm::vec3 getUp() const;

        [[nodiscard]] glm::vec3 getRight() const;

        [[nodiscard]] float getZoom() const;

        [[nodiscard]] float getNear() const;

        [[nodiscard]] float getFar() const;

        [[nodiscard]] glm::vec2 getAngles() const;

        void setPos(glm::vec3 value);

        void setDir(glm::vec3 value);

        void setUp(glm::vec3 value);

        void setRight(glm::vec3 value);

        void setMouseAngle(glm::vec2 value);
    };
} // core
