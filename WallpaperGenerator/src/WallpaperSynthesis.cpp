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

int WallpaperSynthesis::putCenteredTextOnImg(string imageName, string text, string fontName, string outputPath) {
  // read image in opencv matrix
  cv::Mat img=cv::imread("../img/" + imageName);

  // read settings.json file
  std::ifstream f("../settings.json");
  json settings = json::parse(f);

  // store settings data in proper variables
  int fontHeight=settings.at("fontHeight");
	int thickness=settings.at("thickness");
	int lineStyle=settings.at("lineStyle");
	int baseline=settings.at("baseline");

  // initialize font
  cv::Ptr<cv::freetype::FreeType2> ft2;
	ft2=cv::freetype::createFreeType2();
	ft2->loadFontData("../fonts/" + fontName, 0);

  // set text position to make it be in the center
  cv::Size textSize=ft2->getTextSize(text,fontHeight,thickness,&baseline);
	cv::Point textOrg((img.cols - textSize.width) / 2,
              (img.rows + textSize.height) / 2);
  
  // put the text on the image
  ft2->putText(img, text, textOrg, fontHeight,
             cv::Scalar(255,255,0), thickness, lineStyle, true );
  
  imwrite("../" + outputPath, img);

  return 0;
}

} // YMC
} // WallpaperGenerator
