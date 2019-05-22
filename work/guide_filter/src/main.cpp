#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat guidedfilter(Mat &srcImage, Mat &srcClone, int r, double eps);

int main()
{
    Mat src = imread("c.jpg", ImreadModes::IMREAD_COLOR);
    vector<Mat> vsrc, vrst;
    split(src, vsrc);
    Mat rst;
    for (int i = 0; i < src.channels(); i++)
    {
        Mat tmp;
        vsrc[i].convertTo(tmp, CV_64FC1, 1.0/255.0);
        Mat p = tmp.clone();
        Mat result = guidedfilter(tmp, p, 6, 0.001);
        result.convertTo(result, CV_8UC1, 255.0);
        vrst.push_back(result);
    }
    merge(vrst, rst);
    imshow("origina", src);
    imshow("filter", rst);
    waitKey(0);
    return 0;
}

Mat guidedfilter(Mat &srcImage, Mat &srcClone, int r, double eps)
{
    srcImage.convertTo(srcImage, CV_64FC1);
    srcClone.convertTo(srcClone, CV_64FC1);
    int NumRows = srcImage.rows;
    int NumCols = srcImage.cols;
    Mat boxResult;
    boxFilter(Mat::ones(NumRows, NumCols, srcImage.type()), boxResult, CV_64FC1, Size(r, r));
    Mat mean_I;
    boxFilter(srcImage, mean_I, CV_64FC1, Size(r, r));
    Mat mean_P;
    boxFilter(srcClone, mean_P, CV_64FC1, Size(r, r));
    Mat mean_IP;
    boxFilter(srcImage.mul(srcClone), mean_IP, CV_64FC1, Size(r, r));
    Mat cov_IP = mean_IP - mean_I.mul(mean_P);
    Mat mean_II;
    boxFilter(srcImage.mul(srcImage), mean_II, CV_64FC1, Size(r, r));
    Mat var_I = mean_II - mean_I.mul(mean_I);
    Mat var_IP = mean_IP - mean_I.mul(mean_P);
    Mat a = cov_IP / (var_I + eps);
    Mat b = mean_P - a.mul(mean_I);
    Mat mean_a;
    boxFilter(a, mean_a, CV_64FC1, Size(r, r));
    mean_a = mean_a / boxResult;
    Mat mean_b;
    boxFilter(b, mean_b, CV_64FC1, Size(r, r));
    mean_b = mean_b / boxResult;
    Mat resultMat = mean_a.mul(srcImage) + mean_b;
    return resultMat;
}
