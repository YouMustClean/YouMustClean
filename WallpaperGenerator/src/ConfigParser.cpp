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

#include "ConfigParser.hpp"

#include <toml.hpp>

using namespace std;
using namespace cv;

namespace YMC {
namespace WallpaperGenerator {
namespace ConfigParser {

int parseCanvasSizeRelatedNumber(std::string number, int canvas_side_length)
{
    if (number.back() == "%")
    {
        double percentage = std::stod(number.substr(0, number.length - 1));
        return (int)(percentage * canvas_side_length);
    }
    if (number == "center")
    {
        return (int)(0.5f * canvas_side_length);
    }
}


int position2offset(int position, int element_size)
{
    if (element_size % 2 != 0)
        return position - (int)(element_size / 2);
    else
        return position - ((element_size / 2) - 1);
}


int parseText(const string & statement)
{
    return statement;
}


int hex2int(const string & hex)
{
    int result = 0;
    for (int i = 0; i < hex.length; ++i)
    {
        int number;
        char char_num = hex[hex.length - 1 - i];
        if (char_num > 0x2f && char_num < 0x3a)
        {
            number = char_num - 0x30;
        }
        else if (char_num > 0x40 && char_num < 0x47)
        {
            number = char_num - 0x40 + 9;
        }
        else
        {
            throw exception("Wrong hex number!");
        }
        result += (1 << (4 * i)) * (number);
    }
    return result;
}


cv::Mat generateFromTOML(const std::string & toml_path)
{
    const auto toml_data = toml::parse(toml_path);

    const string font_path = toml::find<string>(toml_data, "default_font");

    // Canvas setting(s)
    const string canvas_image_source = toml::find<string>(toml_data, "image_source");
    Mat wallpaper(canvas_image_source);
 
    // Iterate the array of elements
    const auto array_of_elements = toml::find(data, "elements");
    for (int i = 0; ; ++i)
    {
        const auto element = toml::find(array_of_elements, i);
        string element_type = toml::find<string>(element, "type");

        if (element_type == "image")
        {
            Mat image(toml::find<string>(element, "path"));

            const auto position = toml::find(element, "position");

            const string position_x = toml::find<string>(position, "x");
            const string position_y = toml::find<string>(position, "y");
            Point offset;
            offset.x = parseCanvasSizeRelatedNumber(position_x, wallpaper.cols);
            offset.x = position2offset(offset.x, image.cols);
            offset.y = parseCanvasSizeRelatedNumber(position_y, wallpaper.rows);
            offset.y = position2offset(offset.y, image.rows);

            // Render this element
            Renderer::putImage(image, offset, wallpaper);
        }
        else if (element_type == "text")
        {
            TextConfig text_config;

            string content = toml::find<string>(element, "value");
            content = parseText(content);

            const string height_str = toml::find<string>(element, "height");
            const string color_str = toml::find<string>(element, "color");

            const string position_x = toml::find<string>(position, "x");
            const string position_y = toml::find<string>(position, "y");
            Point offset;
            offset.x = parseCanvasSizeRelatedNumber(position_x, wallpaper.cols);
            //offset.x = position2offset(offset.x, image.cols);
            offset.y = parseCanvasSizeRelatedNumber(position_y, wallpaper.rows);
            //offset.y = position2offset(offset.y, image.rows);
            ///< Any way to get the image size before render it?
            ///< Or change what the offset's representation to the center's position for the text?
            
            text_config.content = content;
            text_config.fontPath = font_path;
            text_config.height = parseCanvasSizeRelatedNumber(height_str, wallpaper.rows);
            text_config.color = Scalar(hex2int(color_str.substr(1, 2)),
                                       hex2int(color_str.substr(3, 2)),
                                       hex2int(color_str.substr(5, 2)));
            text_config.offset = offset;

            // Render this element
            Renderer::putText(text_config, wallpaper);
        }
    }
}

} // ConfigParser
} // WallpaperGenerator
} // YMC

