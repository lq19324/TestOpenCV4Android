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

int nativeBilateralFilter(Mat& rSrcMat, Mat& rDstMat, int value);


extern "C"
JNIEXPORT jint JNICALL Java_com_example_lq_testopencv_OpenCVHelper_nativeAdaptiveThreshold(
        JNIEnv *env, jclass obj, jobject bitmap, jint method, jint type, jint blockSize, jint param) {

    AndroidBitmapInfo dstBitmapInfo;
    int result = AndroidBitmap_getInfo(env, bitmap, &dstBitmapInfo);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (dstBitmapInfo.width <= 0 || dstBitmapInfo.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != dstBitmapInfo.format) {
        return 0;
    }

    int width = dstBitmapInfo.width;
    int height = dstBitmapInfo.height;
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeAdaptiveThreshold bitmap size=%dx%d", width, height);

    void* dstRgba = 0;
    AndroidBitmap_lockPixels(env, bitmap, &dstRgba);
    Mat dstMat(height, width, CV_8UC4, dstRgba);

    int retVal = 0;
    Mat gray;
    cvtColor(dstMat, gray, CV_RGBA2GRAY);
    adaptiveThreshold(gray, gray, 255, method, type, blockSize, param);
    cvtColor(gray, dstMat, COLOR_GRAY2RGBA);

    AndroidBitmap_unlockPixels(env, bitmap);
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeAdaptiveThreshold end");
    return retVal;
}

extern "C"
JNIEXPORT jint JNICALL Java_com_example_lq_testopencv_OpenCVHelper_nativeThreshold(
        JNIEnv *env, jclass obj, jobject bitmap, jint type, jint thre) {

    AndroidBitmapInfo dstBitmapInfo;
    int result = AndroidBitmap_getInfo(env, bitmap, &dstBitmapInfo);
    if (ANDROID_BITMAP_RESULT_SUCCESS != result) {
        return 0;
    }
    if (dstBitmapInfo.width <= 0 || dstBitmapInfo.height <= 0 || ANDROID_BITMAP_FORMAT_RGBA_8888 != dstBitmapInfo.format) {
        return 0;
    }

    int width = dstBitmapInfo.width;
    int height = dstBitmapInfo.height;
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeThreshold bitmap size=%dx%d", width, height);

    void* dstRgba = 0;
    AndroidBitmap_lockPixels(env, bitmap, &dstRgba);
    Mat dstMat(height, width, CV_8UC4, dstRgba);

    int retVal = 0;
    Mat gray;
    cvtColor(dstMat, gray, CV_RGBA2GRAY);
    threshold(gray, gray, thre, 255, type);
    cvtColor(gray, dstMat, COLOR_GRAY2RGBA);

    AndroidBitmap_unlockPixels(env, bitmap);
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeThreshold end");
    return retVal;
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
    } else if (type == 5) {// bilateral blur
        retVal = nativeBilateralFilter(rSrcMat, rDstMat, level);
    }

    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage retVal=%d", retVal);
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage rDstMat type=%d rSrcMat type=%d", rDstMat.type(), rSrcMat.type());
    LOG("Java_com_example_lq_testopencv_OpenCVHelper_nativeProcessImage CV_8UC1=%d CV_8UC4=%d",CV_8UC1, CV_8UC4);

    AndroidBitmap_unlockPixels(env, srcBitmap);
    AndroidBitmap_unlockPixels(env, dstBitmap);

    return retVal;
}


int nativeGray(Mat& rSrcMat, Mat& rDstMat) {
    Mat gray;
    cvtColor(rSrcMat, gray, CV_RGBA2GRAY); // Assuming RGBA input
    cvtColor(gray, rDstMat, COLOR_GRAY2RGBA);

    if (rDstMat.rows == rSrcMat.rows && rDstMat.cols == rSrcMat.cols) {
        return (1);
    }
    return(0);
}

/*中值平滑
 * 卷积内中间值
 * */
int nativeMediumBlur(Mat& rSrcMat, Mat& rDstMat, int ksize) {
    medianBlur(rSrcMat, rDstMat, ksize);
    LOG("nativeMediumBlur() rDstMat type=%d", rDstMat.type());
    return (1);
}

/*均值平滑
 * 卷积内求平均
 * */
int nativeBlur(Mat& rSrcMat, Mat& rDstMat, Size_<int>& ksize) {
    blur(rSrcMat, rDstMat, ksize);
    LOG("nativeBlur() rDstMat type=%d", rDstMat.type());
    return 1;
}
/*
 * 高斯平滑
 * 滤波算法中，目标点上的像素值通常是由其所在位置上的周围的一个小局部邻居像素的值所决定。
 * 在2D高斯滤波中的具体实现就是对周围的一定范围内的像素值分别赋以不同的高斯权重值，并在加权平均后得到当前点的最终结果。
 * 而这里的高斯权重因子是利用两个像素之间的空间距离（在图像中为2D）关系来生成。
 * 通过高斯分布的曲线可以发现，离目标像素越近的点对最终结果的贡献越大，反之则越小。
 * */
int nativeGaussianBlur(Mat& rSrcMat, Mat& rDstMat, Size_<int>& ksize) {
    GaussianBlur(rSrcMat, rDstMat, ksize, 3, 3);
    Mat result;
    GaussianBlur(rDstMat, result, ksize, 3, 3);
    result.copyTo(rDstMat);
    GaussianBlur(rDstMat, result, ksize, 3, 3);
    result.copyTo(rDstMat);
    LOG("nativeGaussianBlur() rDstMat type=%d", rDstMat.type());
    return 1;
}

/*
 * 双边平滑
 * 高斯滤波在低通滤波算法中有不错的表现，但是其却有另外一个问题，那就是只考虑了像素间的空间位置上的关系，
 * 因此滤波的结果会丢失边缘的信息。这里的边缘主要是指图像中主要的不同颜色区域（比如蓝色的天空，黑色的头发等），
 * 而Bilateral就是在Gaussian blur中加入了另外的一个权重分部来解决这一问题。
 * **/
//E/cv::error(): OpenCV Error: Assertion failed ((src.type() == CV_8UC1 || src.type() == CV_8UC3) && src.type() == dst.type() && src.size() == dst.size() && src.data != dst.data) in void cv::bilateralFilter_8u(const cv::Mat&, cv::Mat&, int, double, double, int), file /build/2_4_pack-android/opencv/modules/imgproc/src/smooth.cpp, line 1925
int nativeBilateralFilter(Mat& rSrcMat, Mat& rDstMat, int value) {
    Mat src3Mat;
    cvtColor(rSrcMat, src3Mat, COLOR_RGBA2RGB);
    Mat dst3Mat;
    cvtColor(rDstMat, dst3Mat, COLOR_RGBA2RGB);
    bilateralFilter(src3Mat, dst3Mat, value, value, value*2);
    cvtColor(dst3Mat, rDstMat, COLOR_RGB2RGBA);
    LOG("nativeBilateralFilter() rDstMat type=%d", rDstMat.type());
    return 1;
}

