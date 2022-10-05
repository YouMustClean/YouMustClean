cmake_minimum_required(VERSION 3.16)
project(WallpaperGenerator)

set(CMAKE_CXX_STANDARD 17)

# OpenCV
find_package(OpenCV REQUIRED)
# log.c
set(LOG_C_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../Thirdparty/log.c/src)
set(LOG_C_SRC          ${PROJECT_SOURCE_DIR}/../Thirdparty/log.c/src/log.c)
set(PROJECT_SRC          ${PROJECT_SOURCE_DIR}/../Thirdparty/log.c/src/log.c)
# json
set(JSON_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/../Thirdparty/json)
# FreeType (perhaps not needed since opencv_contrib)
#find_package(Freetype REQUIRED)

# Thirdparty link libraries
set(THIRDPARTY_LIBS
${OpenCV_LIBS}
#${FREETYPE_LIBRARIES}
)

# Include directories
include_directories(
${PROJECT_SOURCE_DIR}/include
${OpenCV_INCLUDE_DIRS}
${LOG_C_INCLUDE_DIRS}
${JSON_INCLUDE_DIRS}
#${FREETYPE_INCLUDE_DIRS}
)

# Build output settings
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# WallpaperGenerator module main program
add_executable(ymc-wallpaper-generator
${PROJECT_SOURCE_DIR}/src/WallpaperSynthesis.cpp
WallpaperGenerator.cpp
${LOG_C_SRC}
)

# Link libraries
target_link_libraries(ymc-wallpaper-generator ${THIRDPARTY_LIBS})
