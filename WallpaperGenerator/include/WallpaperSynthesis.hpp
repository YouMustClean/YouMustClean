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

#include <opencv2/opencv.hpp>
#include <fmt/core.h>

#include <string>
#include <list>
#include <stdexcept>

namespace YMC {
namespace WallpaperGenerator {

class ImageLayer
{
public:
    ImageLayer(const cv::Mat & _src, int _offset_x = 0, int _offset_y = 0);

public:
    cv::Mat image;
    int offset_x;
    int offset_y;

};
  
class WallpaperSynthesis
{
public:

    WallpaperSynthesis();

    ///
    ///  Accessible Properties
    ///

    // canvas
    const cv::Mat & get_canvas() const;
    bool canvasIsValid() const;

    void set_canvas(const cv::Mat & _src);
    void readImageAsCanvas(const std::string & path);


    // layers
    const std::list<ImageLayer> & get_layers() const;
    const ImageLayer & get_layer_at(size_t _i) const;
    size_t get_layer_num() const;

    void addLayer(const ImageLayer & _layer);
    void addLayer(const cv::Mat & _image, int _offset_x = 0, int _offset_y = 0);

private:
    /// Image layers
    cv::Mat                 canvas;
    std::list<ImageLayer>   layers;
    size_t                  layer_num;

};

} // YMC
} // WallpaperGenerator

