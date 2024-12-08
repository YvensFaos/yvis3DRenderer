cmake_minimum_required(VERSION 3.29)
project(yvis3Drenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(yvis3Drenderer main.cpp
        LuaHandler/luahandler.hpp
        LuaHandler/luahandler.cpp
        core/yrenderer.cpp
        core/yrenderer.h
        core/ycamera.cpp
        core/ycamera.h
        core/ymesh.cpp
        core/ymesh.h
        math/YBoundingBox.cpp
        math/YBoundingBox.h
        core/yshader.cpp
        core/yshader.h
        core/ymodel.cpp
        core/ymodel.h
        utils/yluahelper.cpp
        utils/yluahelper.h)

find_package(Lua REQUIRED)
target_include_directories(yvis3Drenderer PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(yvis3Drenderer PRIVATE ${LUA_LIBRARIES})
find_package(GLEW REQUIRED)
target_link_libraries(yvis3Drenderer PRIVATE GLEW::GLEW)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(yvis3Drenderer PRIVATE glfw)
find_package(glm CONFIG REQUIRED)
target_link_libraries(yvis3Drenderer PRIVATE glm::glm)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(yvis3Drenderer PRIVATE assimp::assimp)