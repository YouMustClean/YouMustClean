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
#include <opencv2/calib3d.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>

using namespace cv;
using namespace std;

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
    Mat masked_src(dst.rows, dst.cols, CV_8UC4, Scalar(0)); ///< 8 bit 4 channels RBGA image
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
    }
    if (offset.y < 0)
    {
        src_roi_y -= offset.y;
        src_roi_h = src.rows - src_roi_y;
    }
    Rect src_roi(src_roi_x, src_roi_y, src_roi_w, src_roi_h);
    log_debug("src_roi corners: (%d, %d) (%d, %d)",
                src_roi_x, src_roi_y,
                src_roi_x + src_roi_w, src_roi_y + src_roi_h);

    src(src_roi).copyTo(masked_src(mask_roi));
    log_debug("src is copied to mask_src!");

    /// Extract channels
    Mat out_rgb = Mat::zeros(dst.size(), CV_32FC3);
    Mat src_rgb, src_a;
    Mat dst_rgb, dst_a;
    // Split src
    Mat temp[4];
    masked_src.convertTo(masked_src, CV_32FC4);
    cv::split(masked_src, temp);
    cv::merge(temp, 3, src_rgb);
    temp[0] = temp[1] = temp[2] = temp[3];
    cv::merge(temp, 3, src_a);
    cv::multiply(src_a, Scalar::all(1.0/255), src_a);
    log_debug("splitting channels of src, done");
    log_debug("src_rgb: (%d, %d, %d)", src_rgb.cols, src_rgb.rows, src_rgb.channels());
    log_debug("src_a: (%d, %d, %d)", src_a.cols, src_a.rows, src_a.channels());
    // Split dst
    temp[0] = temp[1] = temp[2] = temp[3] = cv::Mat();
    dst.convertTo(dst, CV_32FC4);
    cv::split(dst, temp);
    cv::merge(temp, 3, dst_rgb);
    temp[0] = temp[1] = temp[2] = temp[3];
    cv::merge(temp, 3, dst_a);
    cv::multiply(dst_a, Scalar::all(1.0/255), dst_a);
    log_debug("splitting channels of dst, done");
    log_debug("dst_rgb: (%d, %d, %d)", dst_rgb.cols, dst_rgb.rows, dst_rgb.channels());
    log_debug("dst_a: (%d, %d, %d)", dst_a.cols, dst_a.rows, dst_a.channels());
    // Alpha blending
    // *** RGB channels ***
    cv::multiply(src_a, src_rgb, src_rgb);
    cv::multiply(Scalar::all(1.0)-src_a, dst_rgb, dst_rgb);
    cv::add(src_rgb, dst_rgb, out_rgb);
    out_rgb.convertTo(out_rgb, CV_8UC3);
    log_debug("blending RGB channels, done");
    // *** alpha channel ***
    temp[0] = temp[1] = temp[2] = temp[3] = cv::Mat();
    cv::split(src_a, temp);
    src_a = temp[0];
    cv::split(dst_a, temp);
    dst_a = temp[0];
    cv::multiply(Scalar::all(1.0)-src_a, dst_a, dst_a);
    cv::add(src_a, dst_a, dst_a);
    cv::multiply(dst_a, Scalar::all(255), dst_a);
    dst_a.convertTo(dst_a, CV_8UC1);
    log_debug("blending alpha channel, done");
    // Merge all
    temp[0] = temp[1] = temp[2] = temp[3] = cv::Mat();
    cv::split(out_rgb, temp);
    temp[3] = dst_a;
    cv::merge(temp, 4, dst);
    log_debug("all channels merged, END OF INVOCATION");
}

/**
 * @brief put text on a destination image.
 */
void putText(TextConfig conf, const cv::Mat &dst) {
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
		textOrg = conf.offset;
	}

	// put text on image
	ft2->putText(dst, conf.content, textOrg, conf.height, conf.color, conf.thickness, conf.lineStyle, false);
}

} // Renderer
} // WallpaperGenerator
} // YMC

