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
	imshow("original", img);
    int channels = img.channels();
    int r, g, b;
    Mat tmp(img.size(), img.type());
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p = img.ptr<uchar>(i);
        uchar* output = tmp.ptr<uchar>(i);
        for (int j = 0; j < img.cols * channels; j += channels)
        {
            uchar* tmp = std::min(&p[j], &p[j + 1]);
            tmp = std::min(tmp, &p[j + 2]);
            output[j] = *tmp;
            output[j + 1] = *tmp;
            output[j + 2] = *tmp;
        }
    }
    Mat drk(img.size(), CV_8UC1);
    cvtColor(tmp, drk, COLOR_BGR2GRAY);
    imshow("step 1", drk);
    int radius = 7;
    Mat drkc(img.size(), CV_8UC1);
    for (int i = radius; i < drk.rows; i += 2 * radius + 1)
    {
        
        for (int j = radius; j < drk.cols; j += 2 * radius + 1)
        {
            uchar minnum;
            minnum = drk.ptr<uchar>(i)[j];
            for (int h = -radius; h < 2 * radius + 1; h++)
            {
                for (int k = - radius; k < 2 * radius + 1; k++)
                {
                    if (i + h < drk.rows && j + k < drk.cols)
                    {
                        minnum = *std::min(&minnum, &drk.ptr<uchar>(i + h)[j + k]);
                    }
                }
            }
            for (int h = -radius; h < 2 * radius + 1; h++)
            {
                for (int k = -radius; k < 2 * radius + 1; k++)
                {
                    if (i + h < drk.rows && j + k < drk.cols)
                    {
                        drkc.ptr<uchar>(i + h)[j + k] = minnum;
                    }
                }
            }
        }
    }
    imshow("step 2", drkc);
    waitKey();
	return 0;
}
