/**
 * This file is part of YMC WallpaperGenerator
 *
 * Copyright (C) 2022 YouMustClean Team
 *
 * YMC WallpaperGenerator is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either version 2
 * of the License, or (at your option) any later version.
 *
 * YMC WallpaperGenerator is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with YMC WallpaperGenerator.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class TextConfig {
public:
	int height;
	Point offset;
	string fontPath, content;
	Scalar color;
	bool isPercent = false;

	// normally won't be changed
	int thickness = -1, lineStyle = 8, baseline = 0;
};


/**
 * YMC::WallpaperGenerator::Configuration
 * @brief In charge of TOML configuration storing and parsing
 */
namespace YMC {
namespace WallpaperGenerator {
namespace Configuration {

/**
 * @brief Parse formatted number (or phrase) in to pixel number
 * @note Input include percentage (% ending), linguistic description
 * (e.g. center), and raw pixel number (which can be directly returned).
 *
 * @return 1-dimensional coordinate, in pixel
 */
int parseCanvaSizeRelatedNumber(std::string number, int canvas_side_length);

class CanvasConfig
{
    static int canvas_width;
    static int canvas_height;

    static std::string path;    ///< path to an image

public:
    static void setPath(std::string path) { CanvasConfig::path = path; }

    static void autoCanvasSize();
    static void setCanvasSize(int width, int height)
    { CanvasConfig::canvas_width = width; CanvasConfig::canvas_height = height; }

    static int getCanvasWidth() { return CanvasConfig::canvas_width; }
    static int getCanvasHeight() { return CanvasConfig::canvas_height; }
};  // CanvasConfig

class ImageConfig
{
    std::string path;

    // Position
    std::string position_x;
    std::string position_y;

public:
    ImageConfig(std::string path, std::string position_x, std::string position_y);

    std::string getPath() { return this->path; }
    cv::Point getPosition();
};  // ImageConfig

} // Configuration
} // WallpaperGenerator
} // YMC

