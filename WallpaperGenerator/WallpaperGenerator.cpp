#include "WallpaperSynthesis.hpp"

using namespace std;

int main()
{
	YMC::WallpaperGenerator::WallpaperSynthesis synthesizer;

	synthesizer.putCenteredTextOnImg("lake.jpg", "你好", "MengxiangHeijian.ttf", "output.jpg");

	return 0;
}