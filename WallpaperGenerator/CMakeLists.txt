cmake_minimum_required(VERSION 3.16)
project(WallpaperGenerator)

set(CMAKE_CXX_STANDARD 17)

# OpenCV
find_package(OpenCV REQUIRED)

# Thirdparty link libraries
set(THIRDPARTY_LIBS
${OpenCV_LIBS}
)

# Include directories
include_directories(
${PROJECT_SOURCE_DIR}/include
${OpenCV_INCLUDE_DIRS}
)

# Build output settings
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# WallpaperGenerator module main program
add_executable(ymc-wallpaper-generator
WallpaperGenerator.cpp
)

# Link libraries
target_link_libraries(ymc-wallpaper-generator ${THIRDPARTY_LIBS})

