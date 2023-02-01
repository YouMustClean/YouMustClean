#include <iostream>
#include "Renderer.hpp"
#include <opencv2/opencv.hpp>
using namespace YMC::WallpaperGenerator::Renderer;
using namespace cv;

int main() {
    printf("Hello!\n");
    Mat src = imread("../../testymc/yaya.png");
    Mat background = imread("../../testymc/sun.jpg");
    cvtColor(src, src, COLOR_RGB2RGBA);
    cvtColor(background, background, COLOR_RGB2RGBA);
    putImage(src, Point(1500, -500), background);
    log_debug("background: (%d, %d, %d)", background.cols, background.rows, background.channels());
    cv::imwrite("output.png", background); 
    return 0;
}
