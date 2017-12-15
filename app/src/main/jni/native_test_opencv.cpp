//
// Created by liuqiang on 17-11-23.
//
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Log.h"

using namespace cv;

int nativeGray(Mat& img, Mat& gray);

int nativeMediumBlur(Mat& img, Mat& gray, int size);

int nativeBlur(Mat& img, Mat& gray, Size_<int>& size);

int nativeGaussianBlur(Mat& img, Mat& gray, Size_<int>& size);


extern "C"
JNIEXPORT jint JNICALL Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage(
        JNIEnv *env, jclass obj, jobject srcBitmap, jobject dstBitmap, jint jtype) {

    AndroidBitmapInfo srcBitmapInfo;
    int result = AndroidBitmap_getInfo(env, srcBitmap, &srcBitmapInfo);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (srcBitmapInfo.width <= 0 || srcBitmapInfo.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != srcBitmapInfo.format) {
        return 0;
    }

    AndroidBitmapInfo dstBitmapInfo;
    result = AndroidBitmap_getInfo(env, dstBitmap, &dstBitmapInfo);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (dstBitmapInfo.width <= 0 || dstBitmapInfo.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != dstBitmapInfo.format) {
        return 0;
    }
    if (dstBitmapInfo.width != srcBitmapInfo.width || dstBitmapInfo.height != srcBitmapInfo.height) {
        return 0;
    }

    int width = srcBitmapInfo.width;
    int height = srcBitmapInfo.height;
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage bitmap size=%dx%d", width, height);

    void* srcRgba = 0;
    //unsigned char* srcRgba = 0;
    AndroidBitmap_lockPixels(env, srcBitmap, &srcRgba);

    Mat srcMat(height, width, CV_8UC4, srcRgba);

    void* dstRgba = 0;
    //unsigned char* dstRgba = 0;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstRgba);

    Mat dstMat(height, width, CV_8UC4, dstRgba);

    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage dstRgba=%d", dstRgba);

    Mat& rSrcMat = srcMat;
    Mat& rDstMat = dstMat;

    int retVal = 0;

    int type = jtype;
    type = type >> 8;
    int type2 = jtype;
    int level = type2 & 0xFF;
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage filter type=%d level=%d", type, level);
    if (type == 1) {// gray
        retVal = nativeGray(rSrcMat, rDstMat);
    } else if (type == 2) {// medium blur
        retVal = nativeMediumBlur(rSrcMat, rDstMat, level);
    } else if (type == 3) {// blur
        Size_<int> size(level, level);
        retVal = nativeBlur(rSrcMat, rDstMat, size);
    } else if (type == 4) {
        Size_<int> size(level, level);
        retVal = nativeGaussianBlur(rSrcMat, rDstMat, size);
    }

    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage retVal=%d", retVal);
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage rDstMat type=%d rSrcMat type=%d", rDstMat.type(), rSrcMat.type());
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage CV_8UC1=%d CV_8UC4=%d",CV_8UC1, CV_8UC4);

    AndroidBitmap_unlockPixels(env, srcBitmap);
    AndroidBitmap_unlockPixels(env, dstBitmap);

    return retVal;
}


int nativeGray(Mat& rSrcMat, Mat& rDstMat) {
    cvtColor(rSrcMat, rDstMat, CV_RGBA2GRAY); // Assuming RGBA input
    LOG("nativeGray() dstRgba=%d", rDstMat.data);
    if(rDstMat.type() == CV_8UC1) {
        Mat tmp(rDstMat.size(), rSrcMat.type(), rDstMat.data);
        LOG("nativeGray() CV_8UC1 -> RGBA_8888");
        cvtColor(rDstMat, tmp, COLOR_GRAY2RGBA);
    }
    LOG("nativeGray() rDstMat type=%d", rDstMat.type());

    if (rDstMat.rows == rSrcMat.rows && rDstMat.cols == rSrcMat.cols) {
        return (1);
    }
    return(0);
}

int nativeMediumBlur(Mat& rSrcMat, Mat& rDstMat, int ksize) {
    medianBlur(rSrcMat, rDstMat, ksize);
    LOG("nativeMediumBlur() rDstMat type=%d", rDstMat.type());
    return (1);
}

int nativeBlur(Mat& rSrcMat, Mat& rDstMat, Size_<int>& ksize) {
    blur(rSrcMat, rDstMat, ksize);
    LOG("nativeBlur() rDstMat type=%d", rDstMat.type());
    return 1;
}

int nativeGaussianBlur(Mat& rSrcMat, Mat& rDstMat, Size_<int>& ksize) {
    GaussianBlur(rSrcMat, rDstMat, ksize, 3);
    LOG("nativeGaussianBlur() rDstMat type=%d", rDstMat.type());
    return 1;
}

