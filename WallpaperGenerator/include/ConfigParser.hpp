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

#include <opencv2/core.hpp>
#include <string>
#include <exception>

#include "DataTypes.hpp"

namespace YMC {
namespace WallpaperGenerator {
namespace ConfigParser {

/**
 * @brief Parse formatted number (or phrase) in to pixel number
 * @note Input include percentage (% ending), linguistic description
 * (e.g. center), and raw pixel number (which can be directly returned).
 *
 * @return 1-dimensional coordinate, in pixel
 */
int parseCanvasSizeRelatedNumber(std::string number, int canvas_side_length);

int position2offset(int position, int element_size);

int parseText(const string & statement);

int hex2int(const string & hex);

cv::Mat generateFromTOML(const std::string & toml_path);

} // ConfigParser
} // WallpaperGenerator
} // YMC

