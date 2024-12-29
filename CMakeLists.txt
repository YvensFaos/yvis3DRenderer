cmake_minimum_required(VERSION 3.29)
project(yvis3Drenderer)

set(CMAKE_CXX_STANDARD 20)

include_directories(imgui imgui/backends)

# Add source files for ImGui
set(IMGUI_SOURCES
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

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
        utils/yluahelper.h
        elements/yfog.h
        core/ylight.cpp
        core/ylight.h
        core/yscene.cpp
        core/yscene.h
        scenes/yfogscene.cpp
        scenes/yfogscene.h
        elements/yskybox.cpp
        elements/yskybox.h
        scenes/yskyboxscene.cpp
        scenes/yskyboxscene.h
        core/yframebuffer.cpp
        core/yframebuffer.h
        core/yrenderquad.cpp
        core/yrenderquad.h
        scenes/yedgedetectionscene.cpp
        scenes/yedgedetectionscene.h
        scenes/yloadedscene.cpp
        scenes/yloadedscene.h
        core/ymaterial.cpp
        core/ymaterial.h
        core/yuniform.h
        core/yuniform.cpp
        elements/yobject.cpp
        elements/yobject.h
        elements/yrenderobject.cpp
        elements/yrenderobject.h
        core/ymaterialinstance.cpp
        core/ymaterialinstance.h
        core/yuniformvalue.h
        core/ytransform.cpp
        core/ytransform.h
        utils/yglhelper.h
        core/ylightuniform.cpp
        core/ylightuniform.h
        core/ybaseuniformvalue.h
        core/ybaseuniform.cpp
        ${IMGUI_SOURCES}
)

add_custom_command(
        TARGET yvis3Drenderer
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/data/"
        "${CMAKE_CURRENT_BINARY_DIR}/data/"
        COMMENT "Copying data folder")

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
find_package(Stb REQUIRED)
target_include_directories(yvis3Drenderer PRIVATE ${Stb_INCLUDE_DIR})