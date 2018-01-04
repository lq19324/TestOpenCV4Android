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

    public static final int CV_THRESH_BINARY      =0; /* value = value > threshold ? max_value : 0       */
    public static final int CV_THRESH_BINARY_INV  =1; /* value = value > threshold ? 0 : max_value       */
    public static final int CV_THRESH_TRUNC       =2;  /* value = value > threshold ? threshold : value   */
    public static final int CV_THRESH_TOZERO      =3;  /* value = value > threshold ? value : 0           */
    public static final int CV_THRESH_TOZERO_INV  =4;  /* value = value > threshold ? 0 : value           */
    public static final int CV_THRESH_MASK        =7;
    public static final int CV_THRESH_OTSU        =8 ;/* use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CV_THRESH_* values */

    public static final int[] CV_THRESHOLD_TYPE={0,1,2,3,4,7,8};

    public static final int CV_ADAPTIVE_THRESH_MEAN_C  =0;
    public static final int CV_ADAPTIVE_THRESH_GAUSSIAN_C  =1;

    static {
        System.loadLibrary("test_OpenCV");
    }

    public static native int nativeProcessImage(Bitmap bitmap, Bitmap dstBmp, int type);

    public static native int nativeFindContours(Bitmap bitmap, int mode, int method, ArrayList<ArrayList<Point>> contours);

    public static native int nativeThreshold(Bitmap bitmap, int type, int threshold);

    public static native int nativeAdaptiveThreshold(Bitmap bitmap, int method, int type, int blockSize, int param);

    public static native int nativeStitcher(Bitmap bmp1, Bitmap bmp2, Bitmap finalBmp);
}
