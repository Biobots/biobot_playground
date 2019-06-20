#include "algo.h"
#include <math.h>
#include <string.h>
#include <arm_neon.h>
#include <stdlib.h>
extern "C" {
#include "mm_camera_dbg.h"
}
typedef unsigned char uchar;

int* CalRowSum(uchar* data, int* rst, int radius, int width)
{
    //expand row
    uchar rowdata[width + 2 * radius];
    //memset(rowdata, 0, (width + 2 * radius) * sizeof(uchar));
    int i;
    int size = 2 * radius + 1;
    memset(rowdata, *data, radius * sizeof(uchar));
    memset(rowdata + radius + width, *(data + width - 1), radius * sizeof(uchar));
    memcpy(rowdata + radius, data, width * sizeof(uchar));
    //calculate sum
    int tmp = 0;
    uint8x8_t vp;
    uint8x8_t vm;
    int16x8_t vptmp;
    int16x8_t vmtmp;
    int16x8_t vtmp;
    for (i = 0; i < size; i++)
    {
        tmp += rowdata[i];
    }
    rst[0] = tmp;
    for (i = 1; i + 8 < width; i+=8)
    {
        //tmp += rowdata[i + size - 1] - rowdata[i - 1];
        //rst[i] = tmp;
        vp = vld1_u8((const uint8_t *)(rowdata + i + size - 1));
        vm = vld1_u8((const uint8_t *)(rowdata + i - 1));
        vptmp = vreinterpretq_s16_u16(vaddl_u8(vp, vdup_n_u8(0)));
        vmtmp = vreinterpretq_s16_u16(vaddl_u8(vm, vdup_n_u8(0)));
        vtmp = vqsubq_s16(vptmp, vmtmp);
        rst[i] = rst[i - 1] + vtmp[0];
        rst[i + 1] = rst[i] + vtmp[1];
        rst[i + 2] = rst[i + 1] + vtmp[2];
        rst[i + 3] = rst[i + 2] + vtmp[3];
        rst[i + 4] = rst[i + 3] + vtmp[4];
        rst[i + 5] = rst[i + 4] + vtmp[5];
        rst[i + 6] = rst[i + 5] + vtmp[6];
        rst[i + 7] = rst[i + 6] + vtmp[7];
    }
    for (; i < width; i++)
    {
        rst[i] = rst[i - 1] + rowdata[i + size - 1] - rowdata[i - 1];
    }
    return rst;
}

uchar* boxFilter(uchar* src, uchar* rst, int radius, int width, int height)
{
    int i = 0;
    int j = 0;
    //calculate row sum
    int* rowsum = new int[width * height]; //avoid stack overflow
    int size = 2 * radius + 1;
    float scale = 1.0f / (float)(size * size);
    int* colpos[height + 2 * radius];
    for (i = 0; i < height; i++)
    {
        //neon accelerated
        CalRowSum(src + width * i, rowsum + width * i, radius, width);
        colpos[i + radius] = rowsum + width * i;
    }
    for (i = 1; i <= radius; i++)
    {
        colpos[radius - i] = colpos[radius];
        colpos[height + radius + i - 1] = colpos[height + radius - 1];
    }
    //calculate column sum of first row (except row 2*radius)
    int colsum[width];
    memset(colsum, 0, width * sizeof(int));
    //int32x4_t vrowhead;
    //int32x4_t vcolsum;
    int count = width-4;
    for (i = 0; i < size - 1; i++)
    {
        int* rowhead = colpos[i];
        j = 0;
        //LOGE("defog1: %d, %d", j, count);
        //for (j = 0; j + 4 < width; j+=4)
        //{
        //    vrowhead = vld1q_s32((const int32_t *)(rowhead + j));
        //    vcolsum = vld1q_s32((const int32_t *)(colsum + j));
        //    vcolsum = vaddq_s32(vrowhead, vcolsum);
        //    vst1q_s32(colsum + j, vcolsum);
        //}
        {asm volatile(

            "MOV %[j],#0\t\n"
            "LOOP: \t\n"
            "VLD1.I32 {q1},[%[col]]\t\n"
            "VLD1.I32 {q0},[%[row]]!\t\n"
            "VADD.I32 q1,q0,q1\t\n"
            "ADD %[j],%[j],#4\t\n"
            "VST1.I32 {q1},[%[col]]!\t\n"
            "CMP %[j],%[count]\t\n"
            "BLT LOOP\t\n"

            : [j] "+r" (j)
            : [count] "r" (count), [col] "r" (colsum), [row] "r" (rowhead)
            : "q0", "q1"
        );}

        for (; j < width; j++)
        {
            colsum[j] += rowhead[j];
        }
    }
    //calculate column sum by row
    //int32x4_t vadd;
    //int32x4_t vsub;
    //int32x4_t vtmp;
    //float32x4_t vval;
    int val = 0;
    uint32_t scaleI = (uint32_t)(65536 / (size * size)); //2^14
    for (i = 0; i < height; i++)
    {
        uchar* rstrow = rst + width * i;
        int* addpos = colpos[i + size - 1];
        int* subpos = colpos[i];
        j = 0;
        //for (j = 0; j + 4 < width; j+=4)
        //{
        //    vadd = vld1q_s32((const int32_t *)(addpos + j));
        //    vsub = vld1q_s32((const int32_t *)(subpos + j));
        //    vcolsum = vld1q_s32((const int32_t *)(colsum + j));
        //    vtmp = vaddq_s32(vcolsum, vadd);
        //    vval = vmulq_n_f32(vcvtq_f32_s32(vtmp), scale);
        //    rstrow[j] = (uchar)vval[0];
        //    rstrow[j + 1] = (uchar)vval[1];
        //    rstrow[j + 2] = (uchar)vval[2];
        //    rstrow[j + 3] = (uchar)vval[3];
        //    vcolsum = vqsubq_s32(vtmp, vsub);
        //    vst1q_s32(colsum + j, vcolsum);
        //}

        {asm volatile(

            //"MOV %[j],#0\t\n"
            "VDUP.U32 q4,%[scale]\t\n"
            "LOOP1: \t\n"

            "VLD1.U32 {q1},[%[add]]!\t\n"
            "VLD1.U32 {q0},[%[col]]\t\n"
            "VLD1.U32 {q2},[%[sub]]!\t\n"
            "VADD.U32 q6,q0,q1\t\n"
            "VMUL.U32 q3,q6,q4\t\n"
            "VQSHRN.U32 d9,q3,#0x10\t\n"//>>14
            "VSUB.U32 q0,q6,q2\t\n"
            "VST1.U32 {q0},[%[col]]!\t\n"

            "VLD1.U32 {q1},[%[add]]!\t\n"
            "VLD1.U32 {q0},[%[col]]\t\n"
            "VLD1.U32 {q2},[%[sub]]!\t\n"
            "VADD.U32 q6,q0,q1\t\n"
            "VMUL.U32 q3,q6,q4\t\n"
            "VQSHRN.U32 d10,q3,#0x10\t\n"//>>14
            "VSUB.U32 q0,q6,q2\t\n"
            "VST1.U32 {q0},[%[col]]!\t\n"

            "VPADD.U8 d13,d9,d10\t\n"
            "ADD %[j],%[j],#8\t\n"
            "VST1.U8 {d13},[%[rst]]!\t\n"
            "CMP %[j],%[count]\t\n"
            "BLT LOOP1\t\n"

            : [j] "+r" (j)
            : [add] "r" (addpos), [sub] "r" (subpos), [col] "r" (colsum), [scale] "r" (scaleI), [rst] "r" (rstrow), [count] "r" (width-8)
            : "q0", "q1", "q2", "q3", "q4", "d9", "d10", "q6", "d13"
        );}//too much noise, to be modified

        for (; j < width; j++)
        {
            val = colsum[j] + addpos[j];
            rstrow[j] = (uchar)(val * scale);
            colsum[j] = val - subpos[j];
        }
    }
    delete []rowsum;
    return rst;
}

//do not use
float* boxFilter(uchar* src, float* rst, int radius, int width, int height)
{
    int i = 0;
    int j = 0;
    //calculate row sum
    int* rowsum = new int[width * height]; //avoid stack overflow
    int size = 2 * radius + 1;
    float scale = 1.0f / (float)(size * size);
    int* colpos[height + 2 * radius];
    for (i = 0; i < height; i++)
    {
        //neon accelerated
        CalRowSum(src + width * i, rowsum + width * i, radius, width);
        colpos[i + radius] = rowsum + width * i;
    }
    for (i = 1; i <= radius; i++)
    {
        colpos[radius - i] = colpos[radius];
        colpos[height + radius + i - 1] = colpos[height + radius - 1];
    }
    //calculate column sum of first row (except row 2*radius)
    int colsum[width];
    memset(colsum, 0, width * sizeof(int));
    int32x4_t vrowhead;
    int32x4_t vcolsum;
    for (i = 0; i < size - 1; i++)
    {
        int* rowhead = colpos[i];
        for (j = 0; j + 4 < width; j+=4)
        {
            vrowhead = vld1q_s32((const int32_t *)(rowhead + j));
            vcolsum = vld1q_s32((const int32_t *)(colsum + j));
            vcolsum = vaddq_s32(vrowhead, vcolsum);
            vst1q_s32(colsum + j, vcolsum);
        }
        for (; j < width; j++)
        {
            colsum[j] += rowhead[j];
        }
    }
    //calbulate column sum by row
    int32x4_t vadd;
    int32x4_t vsub;
    int32x4_t vtmp;
    //float32x4_t vf;
    //float f[4];
    int t[4];
    float32x4_t vval;
    int val = 0;
    for (i = 0; i < height; i++)
    {
        float* rstrow = rst + width * i;
        int* addpos = colpos[i + size - 1];
        int* subpos = colpos[i];
        for (j = 0; j + 4 < width; j+=4)
        {
            vadd = vld1q_s32((const int32_t *)(addpos + j));
            vsub = vld1q_s32((const int32_t *)(subpos + j));
            vcolsum = vld1q_s32((const int32_t *)(colsum + j));
            vtmp = vaddq_s32(vcolsum, vadd);
            vst1q_s32(t, vtmp);
            //vst1q_s32(t, vtmp);
            //f[0] = (float)t[0];
            //f[1] = (float)t[1];
            //f[2] = (float)t[2];
            //f[3] = (float)t[3];
            vtmp = vld1q_s32((const int32_t *)t);
            //vf = vld1q_f32(vcvtq_f32_s32(vtmp), scale);
            vval = vmulq_n_f32(vcvtq_f32_s32(vtmp), scale);
            vst1q_f32(rstrow, vval);
            vtmp = vld1q_s32((const int32_t *)t);
            vcolsum = vqsubq_s32(vtmp, vsub);
            vst1q_s32(colsum + j, vcolsum);
        }
        for (; j < width; j++)
        {
            val = colsum[j] + addpos[j];
            rstrow[j] = (float)val * scale;
            colsum[j] = val - subpos[j];
        }
    }
    delete []rowsum;
    return rst;
}

void mAdd(float* rst, float* left, float* right, int width, int height)
{
    int i;
    float32x4_t vl;
    float32x4_t vr;
    for (i = 0; i + 4 < width * height; i+=4)
    {
        vl = vld1q_f32((const float32_t *)(left + i));
        vr = vld1q_f32((const float32_t *)(right + i));
        vst1q_f32(rst + i, vmulq_f32(vl, vr));
    }
    for (; i < width * height; i++)
    {
        rst[i] = left[i] + right[i];
    }
}

void mSub(float* rst, float* left, float* right, int width, int height)
{
    int i;
    float32x4_t vl;
    float32x4_t vr;
    for (i = 0; i + 4 < width * height; i+=4)
    {
        vl = vld1q_f32((const float32_t *)(left + i));
        vr = vld1q_f32((const float32_t *)(right + i));
        vst1q_f32(rst + i, vsubq_f32(vl, vr));
    }
    for (; i < width * height; i++)
    {
        rst[i] = left[i] - right[i];
    }
}

void mMul(float* rst, float* left, float* right, int width, int height)
{
    int i;
    float32x4_t vl;
    float32x4_t vr;
    for (i = 0; i + 4 < width * height; i+=4)
    {
        vl = vld1q_f32((const float32_t *)(left + i));
        vr = vld1q_f32((const float32_t *)(right + i));
        vst1q_f32(rst + i, vmulq_f32(vl, vr));
    }
    for (; i < width * height; i++)
    {
        rst[i] = left[i] * right[i];
    }
}

void mMulNum(float* rst, float* left, float right, int width, int height)
{
    int i;
    float32x4_t vl;
    for (i = 0; i + 4 < width * height; i+=4)
    {
        vl = vld1q_f32((const float32_t *)(left + i));
        vst1q_f32(rst + i, vmulq_n_f32(vl, right));
    }
    for (; i < width * height; i++)
    {
        rst[i] = left[i] * right;
    }
}

void mAddNum(float* rst, float* left, float right, int width, int height)
{
    int i;
    float32x4_t vl;
    for (i = 0; i + 4 < width * height; i+=4)
    {
        vl = vld1q_f32((const float32_t *)(left + i));
        vst1q_f32(rst + i, vaddq_f32(vl, vdupq_n_f32(right)));
    }
    for (; i < width * height; i++)
    {
        rst[i] = left[i] + right;
    }
}

//original
uchar* defog_n(uchar* src, uchar* rst, int r, int width, int height)
{
    uchar* avg = new uchar[width * height];
    avg = boxFilter(src, avg, r, width, height);
    uchar alpha = 255;
    float rho = 0.9;
    int i;
    float val;
    for (i = 0; i < width * height; i++)
    {
        val = rho * (float)avg[i];
        val = val > 255 ? 255 : val;
        val = val < 0 ? 0 : val;
        uchar tmp = (uchar)val;
        avg[i] = tmp < src[i] ? tmp : src[i];
        val = ((float)src[i] - (float)avg[i]) / (alpha - (float)avg[i]) * alpha;
        val = val > 255 ? 255 : val;
        val = val < 0 ? 0 : val;
        rst[i] = (uchar)val;
    }
    delete []avg;
    return rst;
}

//neon accelerated
uchar* defog(uchar* src, uchar* rst, int r, float rho, int width, int height, uchar* table)
{
    static uchar* avg = new uchar[width * height];
    avg = boxFilter(src, avg, r, width, height);
    int i;
    uchar tmp;
    int16_t rhoI = (int16_t)(rho * 128);
    uint16_t addr[8]={0};
    for (i = 0; i + 8 < width * height; i += 8)
    {
        {asm volatile(

            "VDUP.U16 q4,%[rho]\t\n"
            "VLD1.U8 {d0},[%[avg]]\t\n"
            "VMOVL.U8 q2,d0\t\n"
            "VLD1.U8 {d1},[%[src]]\t\n"
            "VMUL.U16 q2,q2,q4\t\n"
            "VSHR.U16 q2,q2,#7\t\n"
            "VMOVL.U8 q3,d1\t\n"
            "VMIN.U16 q2,q2,q3\t\n"
            "VSHL.U16 q3,q3,#8\t\n"
            "VADD.U16 q2,q2,q3\t\n"
            "VST1.U16 {q2},[%[addr]]\t\n"

            : 
            : [avg] "r" (avg+i), [src] "r" (src+i), [rho] "r" (rhoI), [addr] "r" (addr)
            : "d0", "d1", "d2", "d3", "q2", "q3", "q4"
        );}
        //LOGE("defog:done,%d,%d",i,addr[0]);
        rst[i] = table[addr[0]];
        rst[i + 1] = table[addr[1]];
        rst[i + 2] = table[addr[2]];
        rst[i + 3] = table[addr[3]];
        rst[i + 4] = table[addr[4]];
        rst[i + 5] = table[addr[5]];
        rst[i + 6] = table[addr[6]];
        rst[i + 7] = table[addr[7]];
    }
    
    for (; i < width * height; i++)
    {
        tmp = rho * avg[i];
        avg[i] = tmp < src[i] ? tmp : src[i];
        rst[i] = table[(src[i]<<8)+avg[i]];
    }
    return rst;
}

uchar* defog(uchar* src, uchar* rst, int r, float rho, int width, int height, int* ctrl)
{
    static uchar* avg = new uchar[width * height];
    static int* offset = new int[width * height];
    uchar alpha = 255;
    float alphaf = (float)alpha;
    //float32x4_t vfavg;
    //float32x4_t vtmp;
    //float32x4_t vs;
    int i;
    int j;
    float val[4];
    float s[4];
    float a[4];
    int vals[4];
    float32x4_t vs;
    float32x4_t vfavg;
    float32x4_t vtmp;
    float32x4_t vl;
    float32x4_t vtmp1;
    if (ctrl)
    {
        avg = boxFilter(src, avg, r, width, height);
        for (i = 0; i + 4 < width * height; i+=4)
        {
            s[0] = (float)src[i];
            s[1] = (float)src[i + 1];
            s[2] = (float)src[i + 2];
            s[3] = (float)src[i + 3];
            a[0] = (float)avg[i];
            a[1] = (float)avg[i + 1];
            a[2] = (float)avg[i + 2];
            a[3] = (float)avg[i + 3];
            vs = vld1q_f32((const float32_t *)s);
            vfavg = vld1q_f32((const float32_t *)a);
            vtmp = vmulq_n_f32(vfavg, rho);
            vl = vminq_f32(vtmp, vs);
            vst1q_f32(a, vl);
            //vl = vld1q_f32((const float32_t *)a);
            //vs = vld1q_f32((const float32_t *)s);
            //vtmp = vsubq_f32(vs, vl);
            vtmp = vsubq_f32(vs, vfavg);
            //vfavg = vld1q_f32((const float32_t *)a);
            vtmp1 = vrecpeq_f32(vsubq_f32(vdupq_n_f32(alphaf), vfavg));
            vfavg = vmulq_f32(vtmp, vtmp1);
            vtmp = vmulq_n_f32(vfavg, alphaf);
            vst1q_f32(val, vtmp);
            for (j = 0; j < 4; j++)
            {
                val[j] = val[j] > 255 ? 255 : val[j];
                val[j] = val[j] < 0 ? 0 : val[j];
                offset[i + j] = (int)val[j] - (int)src[i + j];
                rst[i + j] = (uchar)val[j];
            }
        }
        for (; i < width * height; i++)
        {
            uchar tmp = (uchar)(rho * (float)avg[i]);
            avg[i] = tmp < src[i] ? tmp : src[i];
            float v = ((float)src[i] - (float)avg[i]) / (alpha - (float)avg[i]) * alpha;
            v = v > 255 ? 255 : v;
            v = v < 0 ? 0 : v;
            offset[i] = (int)v - (int)src[i];
            rst[i] = (uchar)v;
        }
        ctrl --;
    }
    else
    {
        for (i = 0; i + 4 < width * height; i+=4)
        {
            vs = vld1q_s32((const int32_t *)(src + i));
            vtmp = vld1q_s32((const int32_t *)(offset + i));
            vtmp1 = vaddq_s32(vs, vtmp);
            vst1q_s32(vals, vtmp1);
            for (j = 0; j < 4; j++)
            {
                //vals[j] = vals[j] > 255 ? 255 : vals[j];
                //vals[j] = vals[j] < 0 ? 0 : vals[j];
                rst[i + j] = (uchar)vals[j];
            }
        }
        for (; i < width * height; i++)
        {
            int v = src[i] + offset[i];
            //v = v > 255 ? 255 : v;
            //v = v < 0 ? 0 : v;
            rst[i] = (uchar)v;
        }
        ctrl ++;
    }
    
    //delete []avg;
    return rst;
}

/*float calT(uchar input)
{
    float t = 1 - 0.95 * (float)input / 255;
    if (t > 0.1)
    {
        return t;
    }
    else
    {
        return 0.1;
    }
    
}

int defog(unsigned char* data, int width, int height)
{
    int radius = 55;
    int size = width * height * sizeof(uchar);
    //calculate dark channel
    uchar* drkChnl = (uchar*)malloc(size);
    uchar globalMin = 0;
    int offset;
    for (int dp = radius; dp + radius < height; dp += 2 * radius + 1)
    {
        for (int cp = radius; cp + radius < width; cp += 2 * radius + 1)
        {
            int A = *(SAT + (dp + radius) * width + cp + radius);
            int B = (dp - radius < 1 || cp - radius < 1) ? 0 : *(SAT + (dp - radius) * width + cp - radius);
            int C = (dp - radius < 1) ? 0 : *(SAT + (dp - radius - 1) * width + cp - radius);
            int D = (cp - radius < 1) ? 0 : *(SAT + (dp - radius) * width + cp - radius - 1);
            uchar minnum = (uchar)((A + B - C - D) / ((2 * radius + 1) * (2 * radius + 1)));
            for (int i = -radius; i < radius + 1; i++)
            {
                for (int j = -radius; j < radius + 1; j++)
                {
                    *(drkChnl + dp * width + cp + i * width + j) = minnum;
                }
            }
            if (globalMin < minnum)
            {
                globalMin = minnum;
                offset = dp * width + cp;
            }
        }
    }
    for (int dp = radius; dp + radius < height; dp += 2 * radius + 1)
    {
        for (int cp = radius; cp + radius < width; cp += 2 * radius + 1)
        {
            uchar minnum = *(data + dp * width + cp);
            for (int i = -radius; i < radius + 1; i++)
            {
                for (int j = -radius; j < radius + 1; j++)
                {
                    uchar* tmp = data + dp * width + cp + i * width + j;
                    minnum = minnum > *(tmp) ? *(tmp) : minnum;
                }
            }
            for (int i = -radius; i < radius + 1; i++)
            {
                for (int j = -radius; j < radius + 1; j++)
                {
                    *(drkChnl + dp * width + cp + i * width + j) = minnum;
                }
            }
            if (globalMin < minnum)
            {
                globalMin = minnum;
                offset = dp * width + cp;
            }
        }
    }
    //calculate alpha
    uchar alpha = 0;
    for (int i = -radius; i < radius + 1; i++)
    {
        for (int j = -radius; j < radius + 1; j++)
        {
            uchar tmp = *(data + offset + i * width + j);
            alpha = alpha > tmp ? alpha : tmp;
        }
    }
    uchar* transp = drkChnl;
    for (; transp < drkChnl + size; transp++)
    {
        *(transp) = (uchar)((float)255 * calT(*(transp)));
    }
    //calculate result
    uchar* processp = drkChnl;
    uchar* rscp = data;
    uchar* rslt = (uchar*)malloc(size);
    uchar* rsltp = rslt;
    float pv;
    for (int i = 0; i < size; i++)
    {
        pv = ((((float)*(rscp + i)) - (float)alpha) * 255 / (float)*(processp + i)) + (float)alpha;
        pv = pv > 255 ? 255 : pv;
        pv = pv < 0 ? 0 : pv;
        *(rsltp + i) = (uchar)pv;
    }
    memcpy(data, rslt, size);
    free(drkChnl);
    free(rslt);
    return (int)alpha;
}*/

//生成高斯核函数
double* CalGSKernal(double* gs, int size, double sigma)
{
    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        gs[i] = exp(-((i - size / 2) * (i - size / 2) / (2 * sigma * sigma)));
        sum += gs[i];
    }
    for (int i = 0; i < size; i++)
    {
        gs[i] /= sum;
    }
    return gs;
}

//生成积分图
int* CalSAT(uchar* data, int* SAT, int width, int height)
{
    //int* SAT = (int*)malloc(width * height * sizeof(int)); //need free
    for (int i = 1; i < height; i++)
    {
        *(SAT + i * width) = *(SAT + (i - 1) * width) + (int)*(data + i * width);
    }
    for (int j = 1; j < width; j++)
    {
        *(SAT + j) = *(SAT + j - 1) + (int)*(data + j);
    }
    for (int i = 1; i < height; i++)
    {
        for (int j = 1; j < width; j++)
        {
            int it = i - 1;
            int jt = j - 1;
            *(SAT + i * width + j) = *(SAT + it * width + j) + *(SAT + i * width + jt) - *(SAT + it * width + jt) + (int)*(data + i * width + j);
        }
    }
    return SAT;
}

//根据积分图获取块内和
int getSumBySat(int* SAT, int leftCol, int leftRow, int rightCol, int rightRow, int width)
{
    int A = *(SAT + rightRow * width + rightCol);
    int B = leftCol < 1 || leftRow < 1 ? 0 : *(SAT + (leftRow - 1) * width + leftCol - 1);
    int C = leftCol < 1 ? 0 : *(SAT + leftRow * width + leftCol - 1);
    int D = leftRow < 1 ? 0 : *(SAT + (leftRow - 1) * width + leftCol);
    int rst = A + B - C - D;
    return rst;
}