//
// Created by Yvens Serpa on 07/12/2024.
//
#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "ycamera.h"

namespace core {
    class YRenderer final {
    public:
        std::string title;
        float width;
        float height;

    private:
        std::shared_ptr<GLFWwindow> window;
        bool firstMouse;
        bool mouseIsClickingLeft;

        float shiftModPower;
        float deltaTime;
        float fps;
        float lastX;
        float lastY;
        float moveForce;
        float mouseSensitivity;

        double accumulator;
        double currentTime;
        double finishFrameTime;

        char titleBuffer[196];
        glm::vec4 clearColor;
        std::shared_ptr<YCamera> camera;

        // std::map<int, std::unique_ptr<AKeyBind>> keysMap;
    public:
        YRenderer(float width, float height, std::string title);

        ~YRenderer();

        void changeClearColor(glm::vec4 newClearColor);

        [[nodiscard]] std::shared_ptr<YCamera> getCamera() const;

        void startFrame();

        void finishFrame();

        void closeRenderer() const;

        [[nodiscard]] float getDeltaTime() const;

        [[nodiscard]] double getAccumulator() const;

        [[nodiscard]] bool isRunning() const;

        [[nodiscard]] std::shared_ptr<GLFWwindow> getWindow() const;

        int getWidth() const;

        int getHeight() const;

        float getFPS() const;

        static void setCullFaces(bool activate);

        // void addKeybind(AKeyBind akeyBind);
    private:
        static void staticKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void staticMouseCallback(GLFWwindow *window, double xpos, double ypos);

        static void staticMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

        void initialize();

        void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        void mouseCallback(GLFWwindow *glfwWindow, double xpos, double ypos);

        void mouseButtonCallback(GLFWwindow *glfWwindow, int button, int action, int mods);
    };
} // core
