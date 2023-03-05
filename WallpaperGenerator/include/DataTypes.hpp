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
#include <stdexcept>
#include <unistd.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

inline void check_file_accessibility(const char *fname)
{
    if (access(fname, F_OK) != 0)
    {
        throw runtime_error("File not found! Path: " + std::string(fname));
    }
    if (access(fname, R_OK) != 0)
    {
        throw runtime_error(
                "File access denied! Unable to read the file! Path: "
                + std::string(fname));
    }
}

inline void check_image_validity(const cv::Mat & image, const char *fname)
{
    check_file_accessibility(fname);
    if (image.empty())
    {
        throw runtime_error("Empty image! Path: " + std::string(fname));
    }
}

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

