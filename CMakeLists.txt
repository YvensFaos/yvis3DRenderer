cmake_minimum_required(VERSION 3.29)
project(yvis3Drenderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(yvis3Drenderer main.cpp
        LuaHandler/luahandler.hpp
        LuaHandler/luahandler.cpp)

find_package(Lua REQUIRED)
target_include_directories(yvis3Drenderer PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(yvis3Drenderer PRIVATE ${LUA_LIBRARIES})