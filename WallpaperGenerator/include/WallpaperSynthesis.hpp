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

#include <opencv2/core/core.hpp>
#include <opencv2/freetype.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <any>

using namespace std;
using namespace cv;

namespace YMC {
namespace WallpaperGenerator {

class WallpaperSynthesis
{
public:
/**
 * get settings data from a json file
 * 
 * settings should include: "fontHeight", "thickness", "fontPath", "colorR", "colorG", and "colorB".
 * 
 * @param settingsPath the file from which the settings should be read
 * @param style the settings style to be applied
 * 
 * @return json data
 */
  json readSettings(string settingsPath, string style);

/**
 * Make an image with the text provided and the map of settings
 * 
 * @param text the text to be put on the image
 * @param settings the map of settings to be applied on the text
 * 
 * @return image with the text on it.
 */ 
  Mat getTextImg(string text, json settings);
};

} // YMC
} // WallpaperGenerator
