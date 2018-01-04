package com.example.lq.testopencv;

import android.graphics.Bitmap;
import android.graphics.Point;

import java.util.ArrayList;
/**
 * Created by lq on 17-11-23.
 */

public class OpenCVHelper {

    // mode
    public static final int CV_RETR_EXTERNAL = 0;
    public static final int CV_RETR_LIST = 1;
    public static final int CV_RETR_CCOMP = 2;
    public static final int CV_RETR_TREE = 3;
    public static final int CV_RETR_FLOODFILL=4;

    // method
    public static final int CV_CHAIN_CODE = 0;
    public static final int CV_CHAIN_APPROX_NONE = 1;
    public static final int CV_CHAIN_APPROX_SIMPLE = 2;
    public static final int CV_CHAIN_APPROX_TC89_L1 = 3;
    public static final int CV_CHAIN_APPROX_TC89_KCOS = 4;
    public static final int CV_LINK_RUNS = 5;

    static {
        System.loadLibrary("test_OpenCV");
    }

    public static native int nativeProcessImage(Bitmap bitmap, Bitmap dstBmp, int type);

    public static native int nativeFindContours(Bitmap bitmap, int mode, int method, ArrayList<ArrayList<Point>> contours);

    public static native int nativeStitcher(Bitmap bmp1, Bitmap bmp3, Bitmap finalBmp);
}
