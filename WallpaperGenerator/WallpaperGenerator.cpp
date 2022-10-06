#include "WallpaperSynthesis.hpp"

using namespace std;
using namespace cv;

int main()
{
	YMC::WallpaperGenerator::WallpaperSynthesis synthesizer;

	Mat img = imread("../img/lake.jpg");

	Mat imgWithText;
	imgWithText = synthesizer.putTextOnImg(img, "你好", "title");
	imgWithText = synthesizer.putTextOnImg(imgWithText, "这是一个副标题", "subtitle");

	imwrite("../output.jpg", imgWithText);

	return 0;
}