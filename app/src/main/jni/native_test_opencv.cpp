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

int toGray(Mat& img, Mat& gray);


extern "C"
JNIEXPORT jint JNICALL Java_com_example_lq_testopencv_OpenCVHelper_grayImage(
        JNIEnv *env, jclass obj, jobject srcBitmap, jobject dstBitmap) {

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
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_grayImage bitmap size=%dx%d", width, height);

    void* srcRgba = 0;
    //unsigned char* srcRgba = 0;
    AndroidBitmap_lockPixels(env, srcBitmap, &srcRgba);

    Mat srcMat(height, width, CV_8UC4, srcRgba);

    void* dstRgba = 0;
    //unsigned char* dstRgba = 0;
    AndroidBitmap_lockPixels(env, dstBitmap, &dstRgba);

    Mat dstMat(height, width, CV_8UC4, dstRgba);

    Mat& tRgb = srcMat;
    Mat& tGray = dstMat;

    int conv;
    jint retVal;

    conv = toGray(tRgb, tGray);
    retVal = (jint)conv;

    Mat tmp(height, width, CV_8UC4, dstRgba);
    if(tGray.type() == CV_8UC1)
    {
        LOG("nMatToBitmap: CV_8UC1 -> RGBA_8888");
        cvtColor(tGray, tmp, COLOR_GRAY2RGBA);
    }

    LOG("Java_com_example_lq_testopencv_OpenCVHelper_grayImage retVal=%d", retVal);
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_grayImage tGray type=%d tRgb type=%d", tGray.type(), tRgb.type());
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_grayImage CV_8UC1=%d CV_8UC4=%d",CV_8UC1, CV_8UC4);

    AndroidBitmap_unlockPixels(env, srcBitmap);
    AndroidBitmap_unlockPixels(env, dstBitmap);

    return retVal;
}


int toGray(Mat& img, Mat& gray)
{
    cvtColor(img, gray, CV_RGBA2GRAY); // Assuming RGBA input

    if (gray.rows == img.rows && gray.cols == img.cols)
    {
        return (1);
    }
    return(0);
}