#ifndef HAL_ALGO_H
#define HAL_ALGO_H

typedef unsigned char uchar;

double* CalGSKernal(double* gs, int size, double sigma);

//??¡¤???
int* CalSAT(uchar* data, int* SAT, int width, int height);
int getSumBySat(int* SAT, int leftCol, int leftRow, int rightCol, int rightRow, int width);

int* CalRowSum(uchar* data, int* rst, int radius, int width);
uchar* boxFilter(uchar* src, uchar* rst, int radius, int width, int height);
float* boxFilter(uchar* src, float* rst, int radius, int width, int height);

void mAdd(float* rst, float* left, float* right, int width, int height);
void mSub(float* rst, float* left, float* right, int width, int height);
void mMul(float* rst, float* left, float* right, int width, int height);
void mMulNum(float* rst, float* left, float right, int width, int height);
void mAddNum(float* rst, float* left, float right, int width, int height);


uchar* defog_n(uchar* src, uchar* rst, int r, int width, int height);
uchar* defog(uchar* src, uchar* rst, int r, float rho, int width, int height, uchar* table);
uchar* defog(uchar* src, uchar* rst, int r, float rho, int width, int height, int* ctrl);

#endif