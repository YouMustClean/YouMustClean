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

extern "C" {
#include <log.h>
}

#include "DataTypes.hpp"

#include <opencv2/freetype.hpp>

#include <string>
#include <vector>

namespace YMC {
namespace WallpaperGenerator {
namespace Renderer {

/**
 * @brief To merge source image on destination image.
 * @note Both param src & dst have to be 4 channels matrix.
 */
void putImage(const cv::Mat & src, cv::Point offset, cv::Mat & dst);

/**
 * @brief put text on a destination image.
 */
void putText(TextConfig conf, cv::Mat &dst);

} // Renderer
} // WallpaperGenerator
} // YMC

