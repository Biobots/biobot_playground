#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string.h>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

float calT(uchar input, uchar alpha)
{
    float t = 1 - 0.85 * (float)input / (float)alpha;
    if (t > 0.1)
    {
        return t;
    }
    else
    {
        return 0.1;
    }
    
}

int main()
{
    Mat img = imread("c.jpg", ImreadModes::IMREAD_COLOR);
	imshow("original", img);
    int channels = img.channels();
    Mat tmp(img.size(), img.type());
    printf("size:%d,%d channel:%d\n", img.rows, img.cols, img.channels());
    double duration;
    clock_t start, finish;
    start = clock();
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p = img.ptr<uchar>(i);
        uchar* output = tmp.ptr<uchar>(i);
        for (int j = 0; j < img.cols * channels; j += channels)
        {
            uchar temp = p[j] > p[j+1] ? p[j+1] : p[j];
            temp = temp > p[j+2] ? p[j+2] : temp;
            output[j] = temp;
            output[j + 1] = temp;
            output[j + 2] = temp;
        }
    }
    Mat drk(img.size(), CV_8UC1);
    cvtColor(tmp, drk, COLOR_BGR2GRAY);
    imshow("step 1", drk);
    int radius = 25;
    Mat drkc(img.size(), CV_8UC1);
    int col, row;
    uchar globalmin = 255;
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
                        minnum = minnum > drk.ptr<uchar>(i + h)[j + k] ? drk.ptr<uchar>(i + h)[j + k] : minnum;
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
            if (minnum < globalmin)
            {
                globalmin = minnum;
                row = i; col = j;
            }
        }
    }
    uchar r = 0;
    uchar g = 0;
    uchar b = 0;
    for (int h = -radius; h < 2 * radius + 1; h++)
    {
        uchar* p = img.ptr<uchar>(row + h);
        for (int k = -radius * channels; k < (2 * radius + 1) * channels; k += channels)
        {
            if (row + h < img.rows && col + k < img.cols * channels)
            {
                if ((p[col + k] + p[col + k + 1] + p[col + k + 2]) > (r + b + g))
                {
                    r = p[col + k + 2];
                    g = p[col + k + 1];
                    b = p[col + k];
                } 
            }
        }
    }
    //r=255; g=255; b=255;
    printf("%d, %d, %d, %d, %d, %d\n", row, col, (int)r, (int)g, (int)b, (int)globalmin);
    imshow("step 2", drkc);
    Mat dst(img.size(), img.type());
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p = img.ptr<uchar>(i);
        uchar* output = dst.ptr<uchar>(i);
        uchar* d = drkc.ptr<uchar>(i);
        for (int j = 0; j < img.cols * channels; j += 3)
        {
            float a = ((p[j] - b) / calT(d[j/3], b) + b);
            float b = ((p[j + 1] - g) / calT(d[j/3 + 1], g) + g);
            float c = ((p[j + 2] - r) / calT(d[j/3 + 2], r) + r);
            output[j] = a > 255 ? 255 : (uchar)a;
            output[j + 1] = b > 255 ? 255 : (uchar)b;
            output[j + 2] = c > 255 ? 255 : (uchar)c;
        }
    }
    finish = clock();
    imshow("step 3", dst);
    duration = (double)(finish - start) / CLOCKS_PER_SEC; 
    printf( "%f seconds\n", duration );
    waitKey();
	return 0;
}
