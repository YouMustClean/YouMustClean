cmake_minimum_required(VERSION 3.16)
project(WallpaperGenerator)

set(CMAKE_CXX_STANDARD 17)


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

#
# -DLOG_USE_COLOR: let log.c display color
#
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g -ggdb")
# Export compile informations
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# OpenCV
find_package(OpenCV REQUIRED)
# log.c
set(LOG_C_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../Thirdparty/log.c/src)
set(LOG_C_SRC          ${PROJECT_SOURCE_DIR}/../Thirdparty/log.c/src/log.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLOG_USE_COLOR")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -D__FILE_SHORT__='\"$(subst $(dir $(abspath $<)),,$(abspath $<))\"'")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O1 -g -ggdb -DLOG_USE_COLOR")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__FILE_SHORT__='\"$(subst $(dir $(abspath $<)),,$(abspath $<))\"'")
# toml11
set(TOML11_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../Thirdparty/toml11/)
# Lua
set(LUA_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../Thirdparty/lua)
set(LUA_LIBS ${PROJECT_SOURCE_DIR}/../Thirdparty/lua/liblua.a)

# Thirdparty link libraries
set(THIRDPARTY_LIBS
${OpenCV_LIBS}
${LUA_LIBS}
-ldl
)

# Include directories
include_directories(
${PROJECT_SOURCE_DIR}/include
${OpenCV_INCLUDE_DIRS}
${LOG_C_INCLUDE_DIRS}
${TOML11_INCLUDE_DIRS}
${LUA_INCLUDE_DIRS}
)

# Build output settings
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# WallpaperGenerator module main program
add_executable(ymc-wallpaper-generator
WallpaperGenerator.cpp
src/Renderer.cpp
src/ConfigParser.cpp
${LOG_C_SRC}
)

# Link libraries
target_link_libraries(ymc-wallpaper-generator ${THIRDPARTY_LIBS})

