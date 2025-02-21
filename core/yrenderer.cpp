//
// Created by Yvens Serpa on 07/12/2024.
//
#include "yrenderer.h"
#include "ycamera.h"
#include "../utils/yglhelper.h"

namespace core {
    YRenderer::YRenderer(const float width, const float height, std::string title) : title(std::move(title)),
        width(width), height(height),
        window(nullptr), firstMouse(true),
        mouseIsClickingLeft(false),
        shiftModPower(1.0f),
        deltaTime(0.0f),
        lastX(width / 2.0f),
        lastY(height / 2.0f),
        moveForce(20.0f),
        mouseSensitivity(5.0f),
        accumulator(0.0),
        currentTime(0.0),
        fps(0.0f),
        finishFrameTime(0.0),
        titleBuffer(""), clearColor(
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
        initialize();
        camera = std::make_shared<YCamera>(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glfwSetWindowUserPointer(window.get(), this);
    }

    YRenderer::~YRenderer() {
        closeRenderer();
    }

    void YRenderer::changeClearColor(const glm::vec4 newClearColor) {
        this->clearColor.r = newClearColor.r;
        this->clearColor.g = newClearColor.g;
        this->clearColor.b = newClearColor.b;
        this->clearColor.a = newClearColor.a;
    }

    std::shared_ptr<YCamera> YRenderer::getCamera() const {
        return camera;
    }

    void YRenderer::startFrame(const GLuint fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, 2 * static_cast<GLsizei>(width), 2 * static_cast<GLsizei>(height));
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        currentTime = glfwGetTime();
        finishFrameTime = 0.0;

        camera->cacheViewProjectionMatrix(width, height);
    }

    void YRenderer::finishFrame() {
        glfwSwapBuffers(window.get());
        glfwPollEvents();

        utils::YGLHelper::checkGLError();

        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;
        accumulator += deltaTime;
        fps = 1.0f / (float) deltaTime;

        snprintf(titleBuffer, 256, "%s - FPS: %6.3f", title.c_str(), fps);
        glfwSetWindowTitle(window.get(), titleBuffer);
    }

    void YRenderer::closeRenderer() const {
        glfwDestroyWindow(window.get());
        glfwTerminate();
    }

    float YRenderer::getDeltaTime() const {
        return deltaTime;
    }

    double YRenderer::getAccumulator() const {
        return accumulator;
    }

    bool YRenderer::isRunning() const {
        return glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.get()) == 0;
    }

    void YRenderer::initialize() {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            return;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        const auto intWidth = static_cast<GLsizei>(width);
        const auto intHeight = static_cast<GLsizei>(height);

        const auto rawWindow = glfwCreateWindow(intWidth, intHeight, title.c_str(), nullptr, nullptr);
        if (rawWindow == nullptr) {
            fprintf(
                stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(rawWindow);
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return;
        }

        window = std::shared_ptr<GLFWwindow>(rawWindow, [](GLFWwindow *deleteWindow) {
            glfwDestroyWindow(deleteWindow);
            glfwTerminate();
        });

        GLint MaxPatchVertices = 0;
        glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
        printf("Max supported patch vertices %d\n", MaxPatchVertices);

        glfwSetKeyCallback(window.get(), YRenderer::staticKeyCallback);
        glfwSetCursorPosCallback(window.get(), YRenderer::staticMouseCallback);
        glfwSetMouseButtonCallback(window.get(), YRenderer::staticMouseButtonCallback);
        glfwSetInputMode(window.get(), GLFW_STICKY_KEYS, GL_TRUE);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void YRenderer::setCullFaces(const bool activate) {
        if (activate) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    std::shared_ptr<GLFWwindow> YRenderer::getWindow() const {
        return std::shared_ptr(window);
    }

    int YRenderer::getWidth() const {
        return static_cast<int>(width);
    }

    int YRenderer::getHeight() const {
        return static_cast<int>(height);
    }

    float YRenderer::getFPS() const {
        return fps;
    }

    // void YRenderer::addKeybind(AKeyBind akeyBind)
    // {
    // 	this->keysMap.try_emplace(akeyBind.getKey(), std::make_unique<AKeyBind>(akeyBind));
    // }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppDFAUnreachableFunctionCall
    void YRenderer::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        // // for(auto it = this->keysMap.cbegin(); it != this->keysMap.cend(); it++) {
        // // 	if (key == (*it).first)
        // // 	{
        // // 		(*it).second->execute(action, mods);
        // // 	}
        // // }

        shiftModPower = 1.0f;
        if (mods == GLFW_MOD_SHIFT) {
            shiftModPower = 2.5f;
        } else if (mods == GLFW_MOD_CONTROL) {
            shiftModPower = 0.5f;
        }
        if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->MoveSideways(shiftModPower * -moveForce * deltaTime);
        }
        if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->MoveSideways(shiftModPower * moveForce * deltaTime);
        }
        if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->MoveForward(shiftModPower * moveForce * deltaTime);
        }
        if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->MoveForward(shiftModPower * -moveForce * deltaTime);
        }
        if ((key == GLFW_KEY_R) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->MoveUp(shiftModPower * moveForce * deltaTime);
        }
        if ((key == GLFW_KEY_F) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->MoveUp(shiftModPower * -moveForce * deltaTime);
        }
        if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->Zoom(5.0f);
        }
        if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->Zoom(-5.0f);
        }
        if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->RotateWithMouse(10, 0);
        }
        if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            camera->RotateWithMouse(-10, 0);
        }

        if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
            printf("Printing Info: \n");
            printf("---------------\n");
            printf("Del: %9f\n", deltaTime);
            printf("Acc: %9f\n", accumulator);
            printf("---------------\n");
           camera->logToConsole();
            printf("---------------\n\n");
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppDFAUnreachableFunctionCall
    void YRenderer::mouseCallback(GLFWwindow *glfwWindow, double xpos, double ypos) {
        if (firstMouse) {
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            firstMouse = false;
        }

        const auto xOffset = static_cast<float>(xpos - lastX);
        const auto yOffset = static_cast<float>(lastY - ypos);
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);

        if (mouseIsClickingLeft) {
            camera->RotateWithMouse(xOffset / mouseSensitivity, yOffset / mouseSensitivity);
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    // ReSharper disable once CppDFAUnreachableFunctionCall
    void YRenderer::mouseButtonCallback(GLFWwindow *glfWwindow, int button, int action, int mods) {
        mouseIsClickingLeft = false;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            mouseIsClickingLeft = true;
        }
    }

    void YRenderer::staticKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        const auto renderer = static_cast<YRenderer *>(glfwGetWindowUserPointer(window));
        renderer->keyCallback(window, key, scancode, action, mods);
    }

    void YRenderer::staticMouseCallback(GLFWwindow *window, double xpos, double ypos) {
        const auto renderer = static_cast<YRenderer *>(glfwGetWindowUserPointer(window));
        renderer->mouseCallback(window, xpos, ypos);
    }

    void YRenderer::staticMouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
        const auto renderer = static_cast<YRenderer *>(glfwGetWindowUserPointer(window));
        renderer->mouseButtonCallback(window, button, action, mods);
    }
} // core
