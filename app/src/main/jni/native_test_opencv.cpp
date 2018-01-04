//
// Created by liuqiang on 17-11-23.
//
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching/stitcher.hpp>

#include "Log.h"

using namespace cv;

int nativeGray(Mat& img, Mat& gray);

int nativeMediumBlur(Mat& img, Mat& gray, int size);

int nativeBlur(Mat& img, Mat& gray, Size_<int>& size);

int nativeGaussianBlur(Mat& img, Mat& gray, Size_<int>& size);


extern "C"
JNIEXPORT jint JNICALL Java_com_example_lq_testopencv_OpenCVHelper_nativeStitcher(
        JNIEnv *env, jclass obj, jobject img1, jobject img2, jobject finalImg) {

    //-------------------111-------------------
    AndroidBitmapInfo bmpInfo1;
    int result = AndroidBitmap_getInfo(env, img1, &bmpInfo1);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (bmpInfo1.width <= 0 || bmpInfo1.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != bmpInfo1.format) {
        return 0;
    }

    int width = bmpInfo1.width;
    int height = bmpInfo1.height;

    void* argb1 = 0;
    AndroidBitmap_lockPixels(env, img1, &argb1);
    Mat stitcherImg1(height, width, CV_8UC4, argb1);

    //-------------------222---------------------
    AndroidBitmapInfo bmpInfo2;
    result = AndroidBitmap_getInfo(env, img2, &bmpInfo2);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (bmpInfo2.width <= 0 || bmpInfo2.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != bmpInfo2.format) {
        return 0;
    }

    width = bmpInfo2.width;
    height = bmpInfo2.height;

    void* argb2 = 0;
    AndroidBitmap_lockPixels(env, img2, &argb2);
    Mat stitcherImg2(height, width, CV_8UC4, argb2);


    vector<Mat> imgs;
    imgs.push_back(stitcherImg1);
    imgs.push_back(stitcherImg2);

    //------------------------------------------
    Mat pano;
    Stitcher stitcher = Stitcher::createDefault(false);
    Stitcher::Status status = stitcher.stitch(imgs, pano);
    if (status != Stitcher::OK)
    {
        LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeStitcher Can't stitch images, error code = %d", status);
        return -1;
    }

    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeStitcher success final img size=%dx%d", pano.cols, pano.rows);
}

extern "C"
JNIEXPORT jint JNICALL Java_com_example_lq_testopencv_OpenCVHelper_nativeFindContours(
        JNIEnv *env, jclass obj, jobject srcBitmap, jint mode, jint method, jobject contours) {

    AndroidBitmapInfo srcBitmapInfo;
    int result = AndroidBitmap_getInfo(env, srcBitmap, &srcBitmapInfo);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (srcBitmapInfo.width <= 0 || srcBitmapInfo.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != srcBitmapInfo.format) {
        return 0;
    }

    int width = srcBitmapInfo.width;
    int height = srcBitmapInfo.height;
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeFindContours bitmap size=%dx%d", width, height);

    void* srcRgba = 0;
    AndroidBitmap_lockPixels(env, srcBitmap, &srcRgba);

    Mat srcMat(height, width, CV_8UC4, srcRgba);

    int retVal = 0;
    Mat gray;
    cvtColor(srcMat, gray, CV_RGBA2GRAY);
    threshold(gray, gray, 120, 255, THRESH_BINARY);

    vector<vector<Point> > cvContours;
    vector<Vec4i> hierarchy;
    findContours(gray, cvContours, hierarchy, mode, method);

    //class ArrayList
    jclass cls_ArrayList = env->FindClass("java/util/ArrayList");
    //construct method in class ArrayList
    jmethodID construct_ArrayList = env->GetMethodID(cls_ArrayList,"<init>","()V");
    //add method in class ArrayList
    jmethodID arrayList_add = env->GetMethodID(cls_ArrayList,"add","(Ljava/lang/Object;)Z");

    // class Point
    jclass cls_Point = env->FindClass("android/graphics/Point");
    //construct method in class Point
    jmethodID construct_Point = env->GetMethodID(cls_Point,"<init>","(II)V");

    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeFindContours cvContours size=%d", cvContours.size());
    for (int i = 0; i < cvContours.size(); ++i) {
        jobject obj_ArrayList = env->NewObject(cls_ArrayList, construct_ArrayList,"");
        int size = cvContours[i].size();
        LOG("%d contour, point size=%d", i, size);
        for (int j = 0; j < size; ++j) {
            jobject obj_Point = env->NewObject(cls_Point, construct_Point, cvContours[i][j].x, cvContours[i][j].y);
            env->CallBooleanMethod(obj_ArrayList, arrayList_add, obj_Point);
            // Warning: 这里如果不手动释放局部引用，很有可能造成局部引用表溢出(JNI ERROR (app bug): local reference table overflow (max=512))
            env->DeleteLocalRef(obj_Point);
        }
        env->CallBooleanMethod(contours, arrayList_add, obj_ArrayList);
        env->DeleteLocalRef(obj_ArrayList);
    }

    AndroidBitmap_unlockPixels(env, srcBitmap);
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeFindContours end");
    return retVal;
}


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
    } else if (type == 4) {// gaussian blur
        Size_<int> size(level, level);
        retVal = nativeGaussianBlur(rSrcMat, rDstMat, size);
    } else if (type == 5) {// find contours

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

