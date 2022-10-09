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

json WallpaperSynthesis::readSettings(string settingsPath, string style) {
  // read settings.json file
  ifstream f("../settings.json");
  json settings = json::parse(f).at(style);
/* 
  // convert json type to map type
  map<string, any> settings;
  for (auto& element : j.items()) {
      std::cout << element.key() << " maps to " << element.value() << std::endl;
      settings[element.key()] = element.value();
  }
 */  
  return settings;
}

Mat WallpaperSynthesis::getTextImg(string text, json settings) {

  // default settings: probably don't need to be changed
  int lineStyle = 8;
  int baseline = 0;

  // store settings data in proper variables
  int fontHeight = settings.at("fontHeight");
  int thickness = settings.at("thickness");
  string fontPath = settings.at("fontPath"); // settings["fontPath"];

  Scalar color(settings.at("colorB"), settings.at("colorG"), settings.at("colorR"));

  // initialize font
  Ptr<freetype::FreeType2> ft2;
	ft2=freetype::createFreeType2();
	ft2->loadFontData(fontPath, 0);

  // create empty image
  Size textSize=ft2->getTextSize(text,fontHeight,thickness, &baseline);
  // Mat img = Mat::zeros(Size(textSize.width,textSize.height),CV_8UC1);
  // Mat img = imread("../img/lake.jpg");
  cv::Mat img(textSize.height * 1.6, textSize.width * 1.1, CV_8UC3, cv::Scalar(0,0,0));
  
  // put the text on the image
  ft2->putText(img, text, Point(0,0), fontHeight,
             color, thickness, lineStyle, false ); // magic number that just works

  return img;
}

} // YMC
} // WallpaperGenerator
