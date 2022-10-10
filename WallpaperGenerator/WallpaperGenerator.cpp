#include "WallpaperSynthesis.hpp"

using namespace std;
using namespace cv;

int main()
{
	YMC::WallpaperGenerator::WallpaperSynthesis synthesizer;

	json settings = synthesizer.readSettings("../settings.json", "title");

	Mat img = synthesizer.getTextImg("你好hello", settings);

	imwrite("../output.jpg", img);

	return 0;
}