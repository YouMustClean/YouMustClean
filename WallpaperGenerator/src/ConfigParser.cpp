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

#include <vector>
#include <exception>

#include <toml.hpp>

using namespace std;
using namespace cv;

namespace YMC {
namespace WallpaperGenerator {
namespace ConfigParser {

float percentage2float(string percentage)
{
    if (percentage.back() != '%')
    {
        log_error("percentage2float(): Percentage not ending with '%%'! Value = {%s}", percentage.c_str());
        char message_buf[128];
        snprintf(message_buf, sizeof(message_buf),
                 "percentage2float(): Percentage not ending with '%%'! Value = {%s}",
                 percentage.c_str());
        throw runtime_error(message_buf);
    }
    float perc_float = std::stof(percentage.substr(0, percentage.find("%")));
    return perc_float / 100.0;
}


void castTheme(const string & canvas_theme, float intensity, cv::Mat & dst)
{
    if (intensity > 1.0 && intensity < 0.0)
    {
        log_fatal("castTheme(): Invalid color theme intensity! Intensity = {%f}", intensity);
        char message_buf[128];
        snprintf(message_buf, sizeof(message_buf),
                 "castTheme(): Invalid color theme intensity! Intensity = {%f}",
                 intensity);
        throw runtime_error(message_buf);
    }

    if (canvas_theme == "dark")
    {
        log_debug("castTheme(): Canvas theme = {dark}, intensity = {%f}", intensity);
        cv::Mat delta = cv::Mat(dst.size(), dst.type());
        cv::multiply(Scalar::all(intensity), dst, delta);
        cv::subtract(dst, delta, dst);
    }
    else if (canvas_theme == "bright")
    {
        log_debug("castTheme(): Canvas theme = {bright}, intensity = {%f}", intensity);
        cv::multiply(Scalar::all(1 - intensity), dst, dst);
        cv::add(dst, Scalar::all(intensity), dst);
    }
    else
    {
        log_error("castTheme(): Invalid theme {%s}!", canvas_theme.c_str());
        char message_buf[128];
        snprintf(message_buf, sizeof(message_buf), "castTheme(): Invalid theme {%s}!", canvas_theme.c_str());
        throw runtime_error(message_buf);
    }
}


int parseCanvasSizeRelatedNumber(string number, int canvas_side_length)
{
    if (number.back() == '%')
    {
        double percentage = std::stod(number.substr(0, number.size() - 1)) / 100;
        return (int)(percentage * canvas_side_length);
    }
    if (number == "center")
    {
        return (int)(0.5f * canvas_side_length);
    }

    return (int)std::stod(number);
}


int position2offset(int position, int element_size)
{
    if (element_size % 2 != 0)
        return position - (int)(element_size / 2);
    else
        return position - (element_size / 2);
}


void loadLuaScriptFile(lua_State *L, const std::string & path_script)
{
    int load_ret = luaL_loadfile(L, path_script.c_str());
    if (load_ret == 0)
    {
        log_info("Lua script (file) successively loaded! Code: %d", load_ret);
    }
    if (load_ret || lua_pcall(L, 0, 0, 0))
    {
        throw runtime_error(
                "Error while loading the Lua script.\nMessage: "
                + std::string(lua_tostring(L, -1)) + "\nCode: "
                + std::to_string(load_ret));
    }
}


void loadLuaScriptString(lua_State *L, const std::string & code)
{
    int load_ret = luaL_loadstring(L, code.c_str());
    if (load_ret == 0)
    {
        log_info("Additional Lua script (string) successively loaded! Code: %d", load_ret);
    }
    if (load_ret || lua_pcall(L, 0, 0, 0))
    {
        throw runtime_error(
                "Error while loading the Lua script.\nMessage: "
                + std::string(lua_tostring(L, -1)) + "\nCode: "
                + std::to_string(load_ret));
    }
}


string parseText(const string & expression, const string & path_additional_script)
{
    string expr = "__expr__ = " + expression;
    // Start a Lua interpreter
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    // Load additional scripts
    if (!path_additional_script.empty())
        loadLuaScriptFile(L, path_additional_script);
    // Load & run
    loadLuaScriptString(L, expr);

    lua_getglobal(L, "__expr__");
    expr = lua_tostring(L, -1);
    return expr;
}


int hex2int(const string & hex)
{
	if (hex.size() != 2)
	{
		throw runtime_error("Wrong Hex Number!");
	}
	if (!isxdigit(hex[0]) || !isxdigit(hex[1]))
	{
		throw runtime_error("Wrong Hex Number!");
	}
	return stoi(hex, nullptr, 16);
}


cv::Mat generateFromTOML(const string & toml_path, const string & path_additional_script)
{
    const auto toml_data = toml::parse(toml_path);

    const string default_font_path = toml::find<string>(toml_data, "default_font");
    check_file_accessibility(default_font_path.c_str());

    // Canvas setting(s)
    const auto & canvas_settings = toml::find(toml_data, "canvas");
    const string canvas_image_source = toml::find<string>(canvas_settings, "image_source");
    Mat wallpaper = imread(canvas_image_source);
    check_image_validity(wallpaper, canvas_image_source.c_str());

    const string canvas_theme = toml::find<string>(canvas_settings, "theme");
    const string theme_color_intensity = toml::find<string>(canvas_settings, "theme_color_intensity");
    const float intensity = percentage2float(theme_color_intensity);
    castTheme(canvas_theme, intensity, wallpaper);
 
    // Iterate the array of elements
    const auto & array_of_elements = toml::find<vector<toml::table>>(toml_data, "elements");
    for (uint32_t i = 0; i < array_of_elements.size(); ++i)
    {
        const toml::value & element = array_of_elements[i];
        string element_type = toml::find<string>(element, "type");

        if (element_type == "image")
        {
            log_debug("Element [%d]: Processing as type \"image\"", i);
            log_debug("Element [%d]: Reading image from '%s'", i, toml::find<string>(element, "path").c_str());
            const string image_path = toml::find<string>(element, "path");
            Mat image = imread(image_path);
            check_image_validity(image, image_path.c_str());

            const auto & position = toml::find(element, "position");

            const string position_x = toml::find<string>(position, "x");
            const string position_y = toml::find<string>(position, "y");
            log_debug("Element [%d]: Center position (raw): (%s, %s)", i, position_x.c_str(), position_y.c_str());
            Point offset;
            offset.x = parseCanvasSizeRelatedNumber(position_x, wallpaper.cols);
            offset.y = parseCanvasSizeRelatedNumber(position_y, wallpaper.rows);
            log_debug("Element [%d]: Center position (parsed): (%d, %d)", i, offset.x, offset.y);
            offset.x = position2offset(offset.x, image.cols);
            offset.y = position2offset(offset.y, image.rows);
            log_debug("Element [%d]: Offset: (%d, %d)", i, offset.x, offset.y);

            // Render this element
            log_debug("Element [%d]: Rendering...", i);
            Renderer::putImage(image, offset, wallpaper);
            log_debug("Element [%d]: Done", i);
        }
        else if (element_type == "text")
        {
            log_debug("Element [%d]: Processing as type \"text\"", i);
            TextConfig text_config;

            string content = toml::find<string>(element, "value");
            log_debug("Element [%d]: Text (raw): {%s}", i, content.c_str());
            content = parseText(content, path_additional_script);
            log_debug("Element [%d]: Text (parsed): {%s}", i, content.c_str());

            const string *font_path = &default_font_path;
            string alternate_font_path = "";
            try
            {
                alternate_font_path = toml::find<string>(element, "font");
                font_path = &alternate_font_path;
            }
            catch (const std::out_of_range & e)
            {
                log_warn("Element [%d]: Font not set, using default font.", i);
                log_debug("Element [%d]: An exception from TOML11 is handled. "
                          "Type: {std::out_of_range}. Error Message:\n%s", i, e.what());
            }

            const string height_str = toml::find<string>(element, "height");
            const string color_str = toml::find<string>(element, "color");
            log_debug("Element [%d]: Height (raw): %s", i, height_str.c_str());
            log_debug("Element [%d]: Color (raw): %s", i, color_str.c_str());

            const auto & position = toml::find(element, "position");

            const string position_x = toml::find<string>(position, "x");
            const string position_y = toml::find<string>(position, "y");
            log_debug("Element [%d]: Center position (raw): (%s, %s)", i, position_x.c_str(), position_y.c_str());
            Point offset;
            offset.x = parseCanvasSizeRelatedNumber(position_x, wallpaper.cols);
            offset.y = parseCanvasSizeRelatedNumber(position_y, wallpaper.rows);
            log_debug("Element [%d]: Center position (parsed): (%d, %d)", i, offset.x, offset.y);
            //offset.x = position2offset(offset.x, image.cols);
            //offset.y = position2offset(offset.y, image.rows);
            ///< Any way to get the image size before render it?
            ///< Or change what the offset's representation to the center's position for the text?
            
            text_config.content = content;
            text_config.fontPath = *font_path;
            text_config.height = parseCanvasSizeRelatedNumber(height_str, wallpaper.rows);
            text_config.color = Scalar(hex2int(color_str.substr(5, 2)),
                                       hex2int(color_str.substr(3, 2)),
                                       hex2int(color_str.substr(1, 2)));
            text_config.offset = offset;

            // Render this element
            log_debug("Element [%d]: Rendering...", i);
            Renderer::putText(text_config, wallpaper);
            log_debug("Element [%d]: Done", i);
        }
    }

    // Post-generation bahaviors
    try
    {
        const auto & post_generation_behaviors = toml::find(toml_data, "post_generation_behaviors");
        const string lua_code = toml::find<string>(post_generation_behaviors, "lua_code");
        // Start a Lua interpreter
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);
        // Load additional scripts
        if (!path_additional_script.empty())
            loadLuaScriptFile(L, path_additional_script);
        // Load & run
        loadLuaScriptString(L, lua_code);

    }
    catch (const std::out_of_range & e)
    {
        log_info("generateFromTOML(): No post-generation behavior is set. Skipping.");
        log_debug("generateFromTOML(): An exception from TOML11 is handled. "
                    "Type: {std::out_of_range}. Error Message:\n%s", e.what());
    }
    return wallpaper;
}

} // ConfigParser
} // WallpaperGenerator
} // YMC

