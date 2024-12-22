//
// Created by Yvens Serpa on 07/12/2024.
//

#include "ycamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace core {
    YCamera::YCamera() : cameraPos(glm::vec3()), cameraTarget(glm::vec3(0.0f, 0.0f, 1.0f)),
                         cameraDirection(glm::vec3(0.0f, 0.0f, 1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
                         cameraRight(glm::vec3(1.0f, 0.0f, 0.0f)), cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                         horizontalAngle(0.0f), verticalAngle(0.0f), zoom(45.0f), near(0.1f), far(1000.0f), cachedViewProjection(0.0f) {
        this->RotateWithMouse(-90.0, 0.0);
    }

    YCamera::YCamera(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 up) : cameraPos(cameraPos),
                                                                                  cameraTarget(cameraTarget), up(up),
                                                                                  zoom(45.0f), horizontalAngle(0.0f),
                                                                                  verticalAngle(0.0f),
                                                                                  near(0.1f), far(1000.0f), cachedViewProjection(0.0f) {
        this->RotateWithMouse(-90.0, 0.0);
    }

    YCamera::~YCamera() = default;

    void YCamera::MoveForward(const float step) {
        const glm::vec3 forwardMovement = this->cameraDirection * step;
        this->cameraPos = this->cameraPos + forwardMovement;
    }

    void YCamera::MoveSideways(const float step) {
        const glm::vec3 sidewaysMovement = this->cameraRight * step;
        this->cameraPos = this->cameraPos + sidewaysMovement;
    }

    void YCamera::MoveUp(const float step) {
        const glm::vec3 upwardMovement = this->cameraUp * step;
        this->cameraPos = this->cameraPos + upwardMovement;
    }

    glm::mat4 YCamera::getView() const {
        return lookAt(this->cameraPos, this->cameraPos + this->cameraDirection, this->cameraUp);
    }

    glm::vec3 YCamera::getPos() const {
        return this->cameraPos;
    }

    glm::vec3 YCamera::getDir() const {
        return this->cameraDirection;
    }

    glm::vec3 YCamera::getUp() const {
        return this->cameraUp;
    }

    glm::vec3 YCamera::getRight() const {
        return this->cameraRight;
    }

    void YCamera::RotateWithMouse(float hAngle, float vAngle) {
        this->horizontalAngle += hAngle;
        this->verticalAngle += vAngle;

        glm::vec3 front;
        front.x = cos(glm::radians(this->horizontalAngle)) * cos(glm::radians(this->verticalAngle));
        front.y = sin(glm::radians(this->verticalAngle));
        front.z = sin(glm::radians(this->horizontalAngle)) * cos(glm::radians(this->verticalAngle));

        this->cameraDirection = normalize(front);
        this->cameraRight = normalize(cross(this->cameraDirection, this->up));
        this->cameraUp = cross(this->cameraRight, this->cameraDirection);
    }

    void YCamera::CalculateRotationFromDirection(const glm::vec3 direction) {
        const float vAngle = glm::degrees(asin(direction.y));
        const int sign = (direction.x < 0.0 || direction.z < 0.0) ? -1 : 1;
        const float hAngle = static_cast<float>(sign) * glm::degrees(acos((direction.x) / cos(glm::radians(vAngle))));

        this->horizontalAngle = hAngle;
        this->verticalAngle = vAngle;
    }

    void YCamera::setMouseAngle(const glm::vec2 value) {
        this->horizontalAngle = value.x;
        this->verticalAngle = value.y;
    }

    void YCamera::Zoom(const float newZoom) {
        this->zoom += newZoom;
        if (this->zoom >= 90.0 || this->zoom <= 0.0) {
            this->zoom -= newZoom;
        }
    }

    float YCamera::getZoom() const {
        return this->zoom;
    }

    float YCamera::getNear() const {
        return this->near;
    }

    float YCamera::getFar() const {
        return this->far;
    }

    void YCamera::setPos(const glm::vec3 value) {
        this->cameraPos.x = value.x;
        this->cameraPos.y = value.y;
        this->cameraPos.z = value.z;
    }

    void YCamera::setDir(const glm::vec3 value) {
        this->cameraDirection.x = value.x;
        this->cameraDirection.y = value.y;
        this->cameraDirection.z = value.z;
    }

    void YCamera::setUp(const glm::vec3 value) {
        this->cameraUp.x = value.x;
        this->cameraUp.y = value.y;
        this->cameraUp.z = value.z;
    }

    void YCamera::setRight(const glm::vec3 value) {
        this->cameraRight.x = value.x;
        this->cameraRight.y = value.y;
        this->cameraRight.z = value.z;
    }

    glm::vec2 YCamera::getAngles() const {
        return {this->horizontalAngle, this->verticalAngle};
    }

    glm::mat4 YCamera::getViewProjectionMatrix(const float width, const float height) const {
        const glm::mat4 view = getView();
        const glm::mat4 projection = glm::perspective(glm::radians(getZoom()),
                                                      width / height, near,
                                                      far);
        const glm::mat4 viewProjection = projection * view;
        return viewProjection;
    }

    void YCamera::cacheViewProjectionMatrix(float width, float height) {
        const glm::mat4 view = getView();
        const glm::mat4 projection = glm::perspective(glm::radians(getZoom()),
                                                      width / height, near,
                                                      far);
        cachedViewProjection = projection * view;
    }

    glm::mat4 YCamera::getCachedViewProjectionMatrix() const {
        return cachedViewProjection;
    }
} // core
