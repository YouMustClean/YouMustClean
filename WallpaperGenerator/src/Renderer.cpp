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

#include "Renderer.hpp"

using namespace cv;

namespace YMC {
namespace WallpaperGenerator{
namespace Renderer {

/**
 * @brief To merge source image on destination image.
 * @note Both param src & dst have to be 4 channels matrix.
 */
void putImage(const cv::Mat & src, cv::Point offset, cv::Mat & dst)
{
    log_debug("putImage INVOKED!");
    log_debug("offset = (%d, %d)", offset.x, offset.y);
    log_debug("src size (%d, %d, %d)", src.cols, src.rows, src.channels());
    log_debug("dst size (%d, %d, %d)", dst.cols, dst.rows, dst.channels());

    /// Create mask of the src
    Mat masked_src(dst.rows, dst.cols, CV_8UC3, Scalar(0)); ///< 8 bit 4 channels RBGA image
    log_debug("masked_src size (%d, %d)", dst.cols, dst.rows);

    int mask_roi_x = offset.x;
    int mask_roi_y = offset.y;
    int mask_roi_w = src.cols;
    int mask_roi_h = src.rows;
    if (offset.x < 0)
    {
        mask_roi_x = 0;
        mask_roi_w = offset.x + src.cols;
    }
    if (offset.y < 0)
    {
        mask_roi_y = 0;
        mask_roi_h = offset.y + src.rows;
    }
    if (offset.x + src.cols > dst.cols)
    {
        mask_roi_w = dst.cols - mask_roi_x;
    }
    if (offset.y + src.rows > dst.rows)
    {
        mask_roi_h = dst.rows - mask_roi_y;
    }
    Rect mask_roi(mask_roi_x, mask_roi_y, mask_roi_w, mask_roi_h);
    log_debug("mask_roi corners: (%d, %d) (%d, %d)",
                mask_roi_x, mask_roi_y,
                mask_roi_x + mask_roi_w, mask_roi_y + mask_roi_h);

    int src_roi_x = 0;
    int src_roi_y = 0;
    int src_roi_w = mask_roi_w;
    int src_roi_h = mask_roi_h;
    if (offset.x < 0)
    {
        src_roi_x -= offset.x;
        src_roi_w = src.cols - src_roi_x;
        if (src_roi_w > dst.cols)
            src_roi_w = mask_roi_w;
    }
    if (offset.y < 0)
    {
        src_roi_y -= offset.y;
        src_roi_h = src.rows - src_roi_y;
        if (src_roi_h > dst.rows)
            src_roi_h = mask_roi_h;
    }
    Rect src_roi(src_roi_x, src_roi_y, src_roi_w, src_roi_h);
    log_debug("src_roi corners: (%d, %d) (%d, %d)",
                src_roi_x, src_roi_y,
                src_roi_x + src_roi_w, src_roi_y + src_roi_h);

    src(src_roi).copyTo(masked_src(mask_roi));
    log_debug("src is copied to mask_src!");

    // Keep masked_src for future updates
    masked_src(mask_roi).copyTo(dst(mask_roi));
}

/**
 * @brief put text on a destination image.
 */
void putText(TextConfig conf, cv::Mat &dst) {
	// initialize font
	Ptr<freetype::FreeType2> ft2;
	ft2 = freetype::createFreeType2();
	ft2->loadFontData(conf.fontPath, 0);

	// set text position
	Size textSize = ft2->getTextSize(conf.content, conf.height, conf.thickness, &conf.baseline);
	Point textOrg;
	if (conf.isPercent) {
		textOrg.x = (dst.cols - textSize.width) * conf.offset.x / 100;
		textOrg.y = (dst.rows - conf.height) * conf.offset.y / 100;
	}
	else {
		textOrg.x = conf.offset.x - textSize.width / 2;
		textOrg.y = conf.offset.y - conf.height / 2;
	}

	// put text on image
    cvtColor(dst, dst, COLOR_RGBA2RGB);
	ft2->putText(dst, conf.content, textOrg, conf.height, conf.color, conf.thickness, conf.lineStyle, false);
    cvtColor(dst, dst, COLOR_RGB2RGBA);
}

} // Renderer
} // WallpaperGenerator
} // YMC

