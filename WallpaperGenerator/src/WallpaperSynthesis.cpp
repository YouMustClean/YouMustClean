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

#include "WallpaperSynthesis.hpp"

namespace YMC {
namespace WallpaperGenerator {

/// class ImageLayer

WallpaperSynthesis::WallpaperSynthesis()
{
    this->layer_num = 0;
}

ImageLayer::ImageLayer(const cv::Mat & _src, int _offset_x, int _offset_y)
{
    this->image = _src;
    this->offset_x = _offset_x;
    this->offset_y = _offset_y;
}

/// class WallpaperSynthesis

void WallpaperSynthesis::readImageAsCanvas(const std::string & path)
{
    this->canvas = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (this->canvas.data == nullptr)
    {
        log_warn("Image \"%s\" is invalid, canvas image is not set.", path.c_str());
    }
}

bool WallpaperSynthesis::canvasIsValid() const
{
    log_debug("Canvas is valid: %s", (this->canvas.data != nullptr) ? "true" : "false");
    return this->canvas.data != nullptr;
}

const std::list<ImageLayer> &
WallpaperSynthesis::get_layers() const
{
    return this->layers;
}

const ImageLayer &
WallpaperSynthesis::get_layer(size_t _i) const
{
    if (_i > this->layer_num)
    {
        log_error("Index out of range! Accessing image layer of index [%u].", _i);
        std::string except_info =
            fmt::format("Index out of range! Accessing image layer of index [{}].", _i);
        throw std::out_of_range(except_info);
    }
    auto it = this->layers.begin();
    size_t it_pos = 0;
    for (; it_pos != _i; ++it_pos)
        ++it;
    log_debug("Returning layer at it_pos = [%u]", it_pos);
    return *it;
}

size_t WallpaperSynthesis::get_layer_num() const
{
    return this->layer_num;
}

void WallpaperSynthesis::addLayer(const ImageLayer & _layer)
{
    this->layers.push_back(_layer);
    ++(this->layer_num);
    log_info((fmt::format("WallpaperSynthesis {}, ", (void*)this) +
              fmt::format("adding a new layer {{Offset = ({}, {})}}. ",
                          _layer.offset_x, _layer.offset_y) +
              fmt::format("Current number of layers: {}.", this->layer_num)).c_str());
}

void WallpaperSynthesis::addLayer(const cv::Mat & _image, int _offset_x, int _offset_y)
{
    ImageLayer _layer = ImageLayer(_image, _offset_x, _offset_y);
    this->layers.push_back(_layer);
    ++(this->layer_num);
    log_info((fmt::format("WallpaperSynthesis {}, ", (void*)this) +
              fmt::format("adding a new layer {{Offset = ({}, {})}}. ",
                          _offset_x, _offset_y) +
              fmt::format("Current number of layers: {}.", this->layer_num)).c_str());
}

} // YMC
} // WallpaperGenerator

