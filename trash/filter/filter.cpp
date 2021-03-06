// filter.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/core/core.hpp> 
#include <opencv2/opencv.hpp> 
#include <string.h>
using namespace cv;
using namespace std;

Mat DFT(const Mat& src)
{
	int w = getOptimalDFTSize(src.cols);
	int h = getOptimalDFTSize(src.rows);
	Mat padded;
	copyMakeBorder(src, padded, 0, h - src.rows, 0, w - src.cols, BORDER_CONSTANT, Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);
	for (int i = 0; i < padded.rows; i++)
	{
		float *ptr = padded.ptr<float>(i);
		for (int j = 0; j < padded.cols; j++)	ptr[j] *= pow(-1, i + j);
	}
	Mat planes[] = { padded,Mat::zeros(padded.size(),CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg);
	dft(complexImg, complexImg);
	/*split(complexImg, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat mag = planes[0];
	mag += Scalar::all(1);
	log(mag, mag);
	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;
	Mat q0(mag, Rect(0, 0, cx, cy));
	Mat q1(mag, Rect(cx, 0, cx, cy));
	Mat q2(mag, Rect(0, cy, cx, cy));
	Mat q3(mag, Rect(cx, cy, cx, cy));
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	normalize(mag, mag, 1, 0, CV_MINMAX);*/
	return complexImg;
}

Mat iDFT(const Mat& src)
{
	Mat tmp;
	Mat plane[] = { src,Mat::zeros(src.size(),CV_32F) };
	idft(src, tmp);
	split(tmp, plane);
	magnitude(plane[0], plane[1], plane[0]);
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);
	return plane[0];
}

Mat ILPF(const Mat& src)
{
	Mat d = DFT(src);
	int w = d.cols / 2;
	int h = d.rows / 2;
	Mat ILPF(d.size(), CV_32FC2);
	float D0 = 20;
	for (int i = 0; i < d.rows; i++)
	{
		float*p = ILPF.ptr<float>(i);
		for (int j = 0; j < d.cols; j++)
		{
			float d = sqrt(pow(i - h, 2) + pow(j - w, 2));
			if (d <= D0)
			{
				p[2 * j] = 1.0f;
				p[2 * j + 1] = 1.0f;
			}
			else 
			{ 
				p[2 * j] = 0.0f;
				p[2 * j + 1] = 0.0f;
			}
		}
	}
	multiply(d, ILPF, ILPF);
	Mat dst = iDFT(ILPF);
	return dst;
}

Mat BLPF(const Mat& src)
{
	Mat d = DFT(src);
	int w = d.cols / 2;
	int h = d.rows / 2;
	int n = 20;
	Mat BLPF(d.size(), CV_32FC2);
	float D0 = 50;
	for (int i = 0; i < d.rows; i++)
	{
		float*p = BLPF.ptr<float>(i);
		for (int j = 0; j < d.cols; j++)
		{
			float d = sqrt(pow(i - h, 2) + pow(j - w, 2));
			p[2 * j] = 1 / (1 + pow(d / D0, 2 * n));
			p[2 * j + 1] = 1 / (1 + pow(d / D0, 2 * n));
		}
	}
	multiply(d, BLPF, BLPF);
	Mat dst = iDFT(BLPF);
	return dst;
}

Mat BLPF(const Mat& src, int n = 1)
{
	Mat d = DFT(src);
	int w = d.cols / 2;
	int h = d.rows / 2;
	Mat BLPF(d.size(), CV_32FC2);
	float D0 = 100;
	for (int i = 0; i < d.rows; i++)
	{
		float*p = BLPF.ptr<float>(i);
		for (int j = 0; j < d.cols; j++)
		{
			float d = sqrt(pow(i - h, 2) + pow(j - w, 2));
			p[2 * j] = 1 / (1 + pow(d / D0, 2 * n));
			p[2 * j + 1] = 1 / (1 + pow(d / D0, 2 * n));
		}
	}
	multiply(d, BLPF, BLPF);
	Mat dst = iDFT(BLPF);
	return dst;
}

Mat gaussianBlur(const Mat& src)
{
	Mat d = DFT(src);
	int w = d.cols / 2;
	int h = d.rows / 2;
	Mat gaussianBlur(d.size(), CV_32FC2);
	float D0 = 2 * 10 * 10;
	for (int i = 0; i < d.rows; i++)
	{
		float*p = gaussianBlur.ptr<float>(i);
		for (int j = 0; j < d.cols; j++)
		{
			float d = pow(i - h, 2) + pow(j - w, 2);
			p[2 * j] = expf(-d / D0);
			p[2 * j + 1] = expf(-d / D0);
		}
	}
	multiply(d, gaussianBlur, gaussianBlur);
	Mat dst = iDFT(gaussianBlur);
	return dst;
}

Mat IHPF(const Mat& src)
{
	Mat d = DFT(src);
	int w = d.cols / 2;
	int h = d.rows / 2;
	Mat IHPF(d.size(), CV_32FC2);
	float D0 = 20;
	for (int i = 0; i < d.rows; i++)
	{
		float*p = IHPF.ptr<float>(i);
		for (int j = 0; j < d.cols; j++)
		{
			float d = sqrt(pow(i - h, 2) + pow(j - w, 2));
			if (d <= D0)
			{
				p[2 * j] = 0.0f;
				p[2 * j + 1] = 0.0f;
			}
			else
			{
				p[2 * j] = 1.0f;
				p[2 * j + 1] = 1.0f;
			}
		}
	}
	multiply(d, IHPF, IHPF);
	Mat dst = iDFT(IHPF);
	return dst;
}

Mat laplaceF(const Mat& src)
{
	Mat d = DFT(src);
	int w = d.cols / 2;
	int h = d.rows / 2;
	Mat laplace(d.size(), CV_32FC2);
	for (int i = 0; i < d.rows ; i++)
	{
		float* output = laplace.ptr<float>(i);
		for (int j = 0; j < d.cols ; j++)
		{
			output[2*j] = -4 * pow(CV_PI, 2) * (pow(i - h, 2) + pow(j - w, 2));
			output[2 * j+1] = -4 * pow(CV_PI, 2) * (pow(i - h, 2) + pow(j - w, 2));
		}
	}
	multiply(d, laplace, laplace);
	Mat dst = iDFT(laplace);
	return dst;
}

Mat laplace(const Mat& src)
{
	int channels = src.channels();
	Mat dst(src.size(), src.type());
	for (int i = 1; i < src.rows - 1; i++)
	{
		const uchar* first = src.ptr<uchar>(i - 1);
		const uchar* second = src.ptr<uchar>(i);
		const uchar* third = src.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i);
		for (int j = channels; j < (src.cols - 1) * channels; j++)
		{
			output[j] = saturate_cast<uchar>(first[j] + third[j] + second[j - 1] + second[j + 1] - 4 * second[j]);
		}
	}
	dst.row(0).setTo(Scalar(0, 0, 0));
	dst.row(dst.rows - 1).setTo(Scalar(0, 0, 0));
	dst.col(0).setTo(Scalar(0, 0, 0));
	dst.col(dst.cols - 1).setTo(Scalar(0, 0, 0));
	dst = src - dst;
	return dst;
}

Mat erode(const Mat& src)
{
	int channels = src.channels();
	Mat dst(src.size(), src.type());
	for (int i = 1; i < src.rows - 1; i++)
	{
		const uchar* first = src.ptr<uchar>(i - 1);
		const uchar* second = src.ptr<uchar>(i);
		const uchar* third = src.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i);
		for (int j = channels; j < (src.cols - 1) * channels; j++)
		{
			vector<uchar> list;
			list.push_back(second[j]);
			list.push_back(second[j - 3]);
			list.push_back(second[j + 3]);
			list.push_back(third[j]);
			list.push_back(third[j + 3]);
			list.push_back(third[j - 3]);
			list.push_back(first[j]);
			list.push_back(first[j + 3]);
			list.push_back(first[j - 3]);
			sort(list.begin(), list.end());
			output[j] = list[0];
		}
	}
	dst.row(0).setTo(Scalar(0, 0, 0));
	dst.row(dst.rows - 1).setTo(Scalar(0, 0, 0));
	dst.col(0).setTo(Scalar(0, 0, 0));
	dst.col(dst.cols - 1).setTo(Scalar(0, 0, 0));
	return dst;
}

Mat dilate(const Mat& src)
{
	int channels = src.channels();
	Mat dst(src.size(), src.type());
	for (int i = 1; i < src.rows - 1; i++)
	{
		const uchar* first = src.ptr<uchar>(i - 1);
		const uchar* second = src.ptr<uchar>(i);
		const uchar* third = src.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i);
		for (int j = channels; j < (src.cols - 1) * channels; j++)
		{
			vector<uchar> list;
			list.push_back(second[j]);
			list.push_back(second[j - 3]);
			list.push_back(second[j + 3]);
			list.push_back(third[j]);
			list.push_back(third[j + 3]);
			list.push_back(third[j - 3]);
			list.push_back(first[j]);
			list.push_back(first[j + 3]);
			list.push_back(first[j - 3]);
			sort(list.begin(), list.end());
			output[j] = list[8];
		}
	}
	dst.row(0).setTo(Scalar(0, 0, 0));
	dst.row(dst.rows - 1).setTo(Scalar(0, 0, 0));
	dst.col(0).setTo(Scalar(0, 0, 0));
	dst.col(dst.cols - 1).setTo(Scalar(0, 0, 0));
	return dst;
}	

Mat sharpen(const Mat& src)
{
	int channels = src.channels();
	Mat dst(src.size(), src.type());
	for (int i = 1; i < src.rows - 1; i++)
	{
		const uchar* first = src.ptr<uchar>(i - 1);
		const uchar* second = src.ptr<uchar>(i );
		const uchar* third = src.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i);
		for (int j = channels; j < (src.cols - 1) * channels; j++)
		{
			output[j] = saturate_cast<uchar>(5 * second[j] - first[j] - second[j - 3] - second[j + 3] - third[j]);
		}
	}
	dst.row(0).setTo(Scalar(0, 0, 0));
	dst.row(dst.rows - 1).setTo(Scalar(0, 0, 0));
	dst.col(0).setTo(Scalar(0, 0, 0));
	dst.col(dst.cols - 1).setTo(Scalar(0, 0, 0));
	return dst;
}

Mat medianBlur(const Mat& src)
{
	int channels = src.channels();
	Mat dst(src.size(), src.type());
	for (int i = 1; i < src.rows - 1; i++)
	{
		const uchar* first = src.ptr<uchar>(i - 1);
		const uchar* second = src.ptr<uchar>(i);
		const uchar* third = src.ptr<uchar>(i + 1);
		uchar* output = dst.ptr<uchar>(i);
		for (int j = channels; j < (src.cols - 1) * channels; j++)
		{
			vector<uchar> list;
			list.push_back(second[j]);
			list.push_back(second[j - 3]);
			list.push_back(second[j + 3]);
			list.push_back(third[j]);
			list.push_back(third[j + 3]);
			list.push_back(third[j - 3]);
			list.push_back(first[j]);
			list.push_back(first[j + 3]);
			list.push_back(first[j - 3]);
			sort(list.begin(), list.end());
			output[j] =list[4];
		}
	}
	dst.row(0).setTo(Scalar(0, 0, 0));
	dst.row(dst.rows - 1).setTo(Scalar(0, 0, 0));
	dst.col(0).setTo(Scalar(0, 0, 0));
	dst.col(dst.cols - 1).setTo(Scalar(0, 0, 0));
	return dst;
}

Mat open(const Mat& src)
{
	Mat tmp = erode(src);
	Mat dst = dilate(tmp);
	return dst;
}

Mat close(const Mat& src)
{
	Mat tmp = dilate(src);
	Mat dst = erode(tmp);
	return dst;
}

Mat topHat(const Mat& src)
{
	Mat openImg = open(src);
	Mat dst = src - openImg;
	return dst;
}

Mat blackHat(const Mat& src)
{
	Mat closeImg = close(src);
	/*Mat dst(src.size(), src.type());
	int channels = src.channels();
	for (int i = 0; i < src.rows; i++)
	{
		const uchar* srcPtr = src.ptr<uchar>(i);
		const uchar* clsPtr = closeImg.ptr<uchar>(i);
		uchar* dstPtr = dst.ptr<uchar>(i);
		for (int j = 0; j < src.cols * channels; j++)
		{
			dstPtr[j] = saturate_cast<uchar>(clsPtr[j] - srcPtr[j]);
		}
	}*/
	Mat dst = closeImg - src;
	return dst;
}

Mat gradient(const Mat& src)
{
	Mat er = erode(src);
	Mat di = dilate(src);
	Mat dst = di - er;
	return dst;
}

Mat(*getFunc(string input))(const Mat&)
{
	if (input == "sharpen") { return sharpen; }
	else if (input == "medianBlur") { return medianBlur; }
	else if (input == "erode") { return erode; }
	else if (input == "dilate") { return dilate; }
	else if (input == "open") { return open; }
	else if (input == "close") { return close; }
	else if (input == "topHat") { return topHat; }
	else if (input == "blackHat") { return blackHat; }
	else if (input == "gradient") { return gradient; }
	else if (input == "laplace") { return laplace; }
	else if (input == "laplaceF") { return laplaceF; }
	else if (input == "ILPF") { return ILPF; }
	else if (input == "BLPF") { return BLPF; }
	else if (input == "gaussianBlur") { return gaussianBlur; }
	else if (input == "IHPF") { return IHPF; }
	else return sharpen;
}

int main(int argc, char** argv)
{
	Mat(*pf)(const Mat&);
	Mat img;
	if (argv[1] == NULL || argv[2] == NULL)
	{
		pf = laplace;
		img= imread("a.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	}
	else
	{
		pf = getFunc(argv[1]);
		if (pf == laplaceF || pf == gaussianBlur || pf == ILPF || pf == static_cast<Mat(*)(const Mat&)>(BLPF) || pf == IHPF)
		{
			img = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
		}
		else
		{
			img = imread(argv[2]);
		}
	}
	imshow("original", img);
	Mat result = pf(img);
	imshow(argv[1], result);
	waitKey();
	return 0;
}
