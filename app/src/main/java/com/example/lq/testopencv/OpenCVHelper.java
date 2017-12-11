package com.example.lq.testopencv;

import android.graphics.Bitmap;
/**
 * Created by lq on 17-11-23.
 */

public class OpenCVHelper {

    static {
        System.loadLibrary("test_OpenCV");
    }

    public static native int nativeProcessImage(Bitmap bitmap, Bitmap dstBmp, int type);
}
