//
// Created by Yvens Serpa on 09/12/2024.
//
#pragma once

#include <GL/glew.h>

#include "yrenderer.h"
#include "../LuaHandler/luahandler.hpp"

namespace core {
    class YScene {
    protected:
        YRenderer &renderer;
        LuaHandler luaHandler;
        int width;
        int height;

        std::unordered_map<std::string, GLint> uniforms;
    public:
        explicit YScene(YRenderer &renderer, const std::string& file, int width, int height);

        YScene(const YScene &scene) = delete;

        YScene &operator=(const YScene &scene) = delete;

        virtual ~YScene() = default;

        void render();

    protected:
        virtual void renderImpl() = 0;
    };
} // core
