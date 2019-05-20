#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string.h>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("b.jpg", ImreadModes::IMREAD_COLOR);
	imshow("compile test", img);
    int channels = img.channels();
    int r, g, b;
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p = img.ptr<uchar>(i);
        for (int j = 0; j < img.cols * channels; j += channels)
        {
            b += (int)p[j];
            g += (int)p[j+1];
            r += (int)p[j+2];
        }
    }
    r /= img.rows * img.cols;
    g /= img.rows * img.cols;
    b /= img.rows * img.cols;
    printf("r:%d, g:%d, b:%d", r, g, b);
    waitKey();
	return 0;
}
