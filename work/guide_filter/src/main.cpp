#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat guidedfilter(Mat &srcImage, Mat &srcClone, int r, double eps);

int* CalRowSum(uchar* data, int* rst, int radius, int width)
{
    //expand row
    uchar rowdata[width + 2 * radius];
    int i;
    int size = 2 * radius + 1;
    for (i = 0; i < radius; i ++)
    {
        rowdata[i] = *data;
    }
    for (i = 0; i < radius; i ++)
    {
        rowdata[i + width] = *(data + width - 1);
    }
    memcpy(rowdata + radius, data, width * sizeof(uchar));
    //calculate sum
    int tmp = 0;
    for (i = 0; i < size; i++)
    {
        tmp += rowdata[i];
    }
    *(rst) = tmp;
    for (i = 1; i < width; i++)
    {
        tmp += rowdata[i + size - 1] - rowdata[i - 1];
        *(rst + i) = tmp;
    }
    return rst;
}

uchar* myBoxFilter(uchar* src, uchar* rst, int radius, int width, int height)
{
    //calculate row sum
    int rowsum[width * height];
    int size = 2 * radius + 1;
    float scale = 1.0f / (float)(size * size);
    int* colpos[height + 2 * radius];
    int i = 0;
    int j = 0;
    for (i = 0; i < height; i++)
    {
        CalRowSum(src + width * i, rowsum + width * i, radius, width);
        colpos[i + radius] = rowsum + width * i;
    }
    for (i = 1; i <= radius; i++)
    {
        colpos[radius - i] = colpos[radius];
        colpos[height + radius + i - 1] = colpos[height + radius - 1];
    }
    //calculate first column
    int colsum[width];
    memset(colsum, 0, width * sizeof(int));
    for (i = 0; i < size - 1; i++)
    {
        int* rowhead = colpos[i];
        for (j = 0; j < width; j++)
        {
            colsum[j] += rowhead[j];
        }
    }
    //calbulate column sum by row
    int val = 0;
    for (i = 0; i < height; i++)
    {
        uchar* rstrow = rst + width * i;
        int* addpos = colpos[i + size - 1];
        int* subpos = colpos[i];
        for (j = 0; j < width; j++)
        {
            val = colsum[j] + addpos[j];
            rstrow[j] = (uchar)(val * scale);
            colsum[j] = val - subpos[j];
        }
    }
    return rst;
}

int main()
{
    Mat gray = imread("d.jpg", ImreadModes::IMREAD_GRAYSCALE);
    //Mat gray(src.rows,src.cols,CV_8UC1,src);
    gray.convertTo(gray, CV_8UC1);
    imshow("original", gray);
    uchar* img = (uchar*)malloc(gray.rows * gray.cols * sizeof(uchar));
    for (int i = 0; i < gray.rows; i++)
    {
        for (int j = 0; j < gray.cols; j++)
        {
            uchar tmp = gray.at<uchar>(i, j);
            img[j + i * gray.cols] = tmp;
        }
    }
    uchar* rst = (uchar*)malloc(gray.rows * gray.cols * sizeof(uchar));
    Mat f;
    rst = myBoxFilter(img, rst, 1, gray.cols, gray.rows);
    //boxFilter(gray, f, CV_8UC1, Size(1, 1));
    f = cv::Mat(gray.rows, gray.cols, CV_8UC1, rst);
    imshow("aft", f);
    free(img);
    free(rst);
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
