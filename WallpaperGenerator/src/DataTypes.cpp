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

#include "DataTypes.hpp"

extern "C" {
#include <log.h>
}

using namespace std;
using namespace cv;

namespace YMC {
namespace WallpaperGenerator {
namespace Configuration {

int parseCanvaSizeRelatedNumber(string number, int canvas_side_length)
{
    return 0;
}


/**
 * Class CanvasConfig
 */

int CanvasConfig::canvas_width = -1;
int CanvasConfig::canvas_height = -1;
std::string CanvasConfig::path = std::string();

void CanvasConfig::autoCanvasSize()
{
    log_debug("CanvasConfig::autoCanvasSize() INVOKED!");
    if (CanvasConfig::path == string())
    {
        log_debug("canvas image path not set, auto size setting failed");
        return;
    }
    else
    {
        Mat canvas_img = imread(CanvasConfig::path);
        CanvasConfig::canvas_width = canvas_img.cols;
        CanvasConfig::canvas_height = canvas_img.rows;
        log_debug("CanvasConfig::canvas_width: %d", CanvasConfig::canvas_width);
        log_debug("CanvasConfig::canvas_height: %d", CanvasConfig::canvas_height);
    }
}


/**
 * Class ImageConfig
 */

ImageConfig::ImageConfig(string path, string position_x, string position_y)
{
    this->path = path;
    this->position_x = position_x;
    this->position_y = position_y;
}

cv::Point ImageConfig::getPosition()
{
    log_debug("ImageConfig::getOffset() INVOKED!");
    Point position;
    position.x = parseCanvaSizeRelatedNumber(this->position_x, CanvasConfig::getCanvasWidth());
    position.y = parseCanvaSizeRelatedNumber(this->position_y, CanvasConfig::getCanvasHeight());
    return position;
}

} // Configuration
} // WallpaperGenerator
} // YMC

