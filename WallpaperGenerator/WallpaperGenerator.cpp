#include "Renderer.hpp"
using namespace YMC;
using namespace WallpaperGenerator;

int main() {
	Mat img = imread("windows.jpg");

	TextConfig conf = {
		200, // height
		{50,50}, // offset
		"../fonts/Microsoft Yahei.ttf", // fontpath
		"你好opencv", // content
		{54, 39, 97}, // color
		true // ispercent
	};
	Renderer::putText(conf, img);

	imwrite("output.jpg", img);

    return 0;
}
