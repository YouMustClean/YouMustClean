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

cv::Mat WallpaperSynthesis::putTextOnImg(cv::Mat img, string text, string style) {

  // read settings.json file
  std::ifstream f("../settings.json");
  json settings = json::parse(f).at(style);

  // store settings data in proper variables
  int fontHeight=settings.at("fontHeight");
	int thickness=settings.at("thickness");
	int lineStyle=settings.at("lineStyle");
	int baseline=settings.at("baseline");
  double textOffsetX=settings.at("textOffsetX");
  double textOffsetY=settings.at("textOffsetY");
  cv::Scalar color(settings.at("colorB"), settings.at("colorG"), settings.at("colorR"));


  // initialize font
  cv::Ptr<cv::freetype::FreeType2> ft2;
	ft2=cv::freetype::createFreeType2();
	ft2->loadFontData(settings.at("fontPath"), 0);

  // set text position
  cv::Size textSize=ft2->getTextSize(text,fontHeight,thickness,&baseline);
	cv::Point textOrg((img.cols - textSize.width) * textOffsetX,
                    (img.rows - fontHeight) * textOffsetY - fontHeight * 0.16); // some magic number that just works
  
  // put the text on the image
  ft2->putText(img, text, textOrg, fontHeight,
             color, thickness, lineStyle, false );

  return img;
}

} // YMC
} // WallpaperGenerator
