#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string.h>
#include <iostream>
#include <time.h>
#include <map>
#include <vector>

using namespace cv;
using namespace std;

float calT(uchar input, uchar alpha)
{
    float t = 1 - 0.95 * (float)input / 255;
    //printf("%f\n", t);
    if (t > 0.1)
    {
        return t;
    }
    else
    {
        return 0.1;
    }
    
}

Mat guidedfilter(Mat &srcImage, Mat &srcClone, int r, double eps, int ddepth)
{
    srcImage.convertTo(srcImage, CV_32FC1);
    srcClone.convertTo(srcClone, CV_32FC1);
    int NumRows = srcImage.rows;
    int NumCols = srcImage.cols;
    Mat boxResult;
    boxFilter(Mat::ones(NumRows, NumCols, srcImage.type()), boxResult, CV_32FC1, Size(r, r));
    Mat mean_I;
    boxFilter(srcImage, mean_I, CV_32FC1, Size(r, r));
    Mat mean_P;
    boxFilter(srcClone, mean_P, CV_32FC1, Size(r, r));
    Mat mean_IP;
    boxFilter(srcImage.mul(srcClone), mean_IP, CV_32FC1, Size(r, r));
    Mat cov_IP = mean_IP - mean_I.mul(mean_P);
    Mat mean_II;
    boxFilter(srcImage.mul(srcImage), mean_II, CV_32FC1, Size(r, r));
    Mat var_I = mean_II - mean_I.mul(mean_I);
    Mat var_IP = mean_IP - mean_I.mul(mean_P);
    Mat a = cov_IP / (var_I + eps);
    Mat b = mean_P - a.mul(mean_I);
    Mat mean_a;
    boxFilter(a, mean_a, CV_32FC1, Size(r, r));
    mean_a = mean_a / boxResult;
    Mat mean_b;
    boxFilter(b, mean_b, CV_32FC1, Size(r, r));
    mean_b = mean_b / boxResult;
    Mat resultMat = mean_a.mul(srcImage) + mean_b;
    resultMat.convertTo(resultMat, ddepth);
    srcImage.convertTo(srcImage, ddepth);
    srcClone.convertTo(srcClone, ddepth);
    return resultMat;
}

bool acompare(const pair<uchar, pair<int, int>> &a, const pair<uchar, pair<int, int>> &b)
{
    return a.first > b.first;
}

int main()
{
    Mat img = imread("k.jpg", ImreadModes::IMREAD_COLOR);
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
    int radius = 11;
    Mat drkb(img.size(), CV_8UC1);
    list<int> lcol, lrow;
    vector<pair<uchar, pair<int, int>>> mitem;
    for (int i = radius; i < drk.rows; i += 2*radius+1)
    {
        for (int j = radius; j < drk.cols; j +=2*radius+1)
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
                        mitem.push_back(make_pair(drk.ptr<uchar>(i + h)[j + k], make_pair(i + h, j + k)));
                    }
                }
            }
            for (int h = -radius; h < 2 * radius + 1; h++)
            {
                for (int k = -radius; k < 2 * radius + 1; k++)
                {
                    if (i + h < drk.rows && j + k < drk.cols)
                    {
                        drkb.ptr<uchar>(i + h)[j + k] = minnum;
                    }
                }
            }
        }
    }
    //Mat drkcc = drkb.clone();
    //Mat drkc = guidedfilter(drk, drkb, 10, 0.01, CV_8UC1);
    uchar r = 255;
    uchar g = 255;
    uchar b = 255;
    int pixelnum = mitem.size() / 1000;
    printf("%d\n", pixelnum);
    std::sort(mitem.begin(), mitem.end(), acompare);
    for (int i = 0; i < pixelnum; i++)
    {
        //printf("%d, %d, %d\n", (int)mitem[i].first, mitem[i].second.first, mitem[i].second.second);
        int row = mitem[i].second.first;
        int col = mitem[i].second.second;
        uchar* p = img.ptr<uchar>(row);
        if ((p[3 * col] + p[3 * col + 1] + p[3 * col + 2]) < (r + b + g))
        {
            r = p[3 * col + 2];
            g = p[3 * col + 1];
            b = p[3 * col];
        }
    }
    Mat gray;
    vector<Mat> vimg;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    split(img, vimg);
    Mat t(img.size(), img.type());
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p = drkb.ptr<uchar>(i);
        uchar* q = t.ptr<uchar>(i);
        for (int j = 0; j < img.cols * channels; j += 3)
        {
            q[j] = 255 * calT(p[j/3], b);
            q[j + 1] = 255 * calT(p[j/3], g);
            q[j + 2] = 255 * calT(p[j/3], r);
        }
    }
    vector<Mat> tv, vrst;
    split(t, tv);
    
    for (int i = 0; i < t.channels(); i++)
    {
        Mat result = guidedfilter(vimg[i], tv[i], 50 * radius, 0.001, CV_8UC1);
        vrst.push_back(result);
    }
    //r=255; g=255; b=255;
    printf("%d, %d, %d, %d\n", (int)r, (int)g, (int)b, pixelnum);
    imshow("step 3", tv[0]);
    Mat rst;
    merge(vrst, rst);
    imshow("step 4", vrst[1]);
    Mat dst(img.size(), img.type());
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p = img.ptr<uchar>(i);
        uchar* output = dst.ptr<uchar>(i);
        uchar* d = rst.ptr<uchar>(i);
        for (int j = 0; j < img.cols * channels; j += channels)
        {
            float a = ((p[j] - b) * 255 / d[j] + b);
            float b = ((p[j + 1] - g) * 255 / d[j + 1] + g);
            float c = ((p[j + 2] - r) * 255 / d[j + 2] + r);
            
            a = a > 255 ? 255 : a;
            b = b > 255 ? 255 : b;
            c = c > 255 ? 255 : c;
            a = a < 0 ? 0 : a;
            b = b < 0 ? 0 : b;
            c = c < 0 ? 0 : c;

            output[j] = (uchar)a;
            output[j + 1] = (uchar)b;
            output[j + 2] = (uchar)c;
            //printf("%d, %d, %d\n", output[j], output[j+1], output[j+2]);
        }
    }
    finish = clock();
    imshow("step 5", dst);
    duration = (double)(finish - start) / CLOCKS_PER_SEC; 
    printf( "%f seconds\n", duration );
    waitKey();
	return 0;
}
