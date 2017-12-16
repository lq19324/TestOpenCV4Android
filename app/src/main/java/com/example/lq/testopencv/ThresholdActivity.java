package com.example.lq.testopencv;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

import java.io.IOException;

/**
 * Created by lq on 17-11-30.
 */

public class ThresholdActivity extends Activity {

    private Bitmap mSrcBitmap;
    private Bitmap mDstBitmap;

    private ImageView mSrcImageView;
    private ImageView mDstImageView;

    private TextView mTimeView;

    private SeekBar mThresholdSeekBar;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_threshold);

        mSrcImageView = (ImageView)findViewById(R.id.src_iv);
        mDstImageView = (ImageView)findViewById(R.id.dst_iv);
        mTimeView = (TextView)findViewById(R.id.tv_time);

        mThresholdSeekBar = (SeekBar)findViewById(R.id.sb_threshold);
        mThresholdSeekBar.setOnSeekBarChangeListener(mSeekListener);

        AssetManager am = getAssets();
        try {
            mSrcBitmap = BitmapFactory.decodeStream(am.open("test_find_contours1.jpg"));
            mDstBitmap = Bitmap.createBitmap(mSrcBitmap.getWidth(), mSrcBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        } catch (IOException e) {
            e.printStackTrace();
        }
        mSrcImageView.setImageBitmap(mSrcBitmap);
    }

    private int threshold = 122;
    private SeekBar.OnSeekBarChangeListener mSeekListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            threshold = seekBar.getProgress();
            mTimeView.setText("type:"+typeString(OpenCVHelper.CV_THRESHOLD_TYPE[typeIdx])+" threshold:"+threshold);
        }
    };

    private int typeIdx = 0;
    public void onButtonClick(View view) {
        int viewID = view.getId();
        if (viewID == R.id.but_type) {
            typeIdx++;
            typeIdx%=7;
            if (view instanceof Button) {
                ((Button)view).setText("type="+typeString(OpenCVHelper.CV_THRESHOLD_TYPE[typeIdx]));
            }
        } else if (viewID == R.id.but_apply) {
            processImage();
        }
    }

    private void processImage() {
        Canvas canvas = new Canvas(mDstBitmap);
        canvas.drawBitmap(mSrcBitmap, 0, 0, null);

        long time = System.currentTimeMillis();
        OpenCVHelper.nativeThreshold(mDstBitmap, OpenCVHelper.CV_THRESHOLD_TYPE[typeIdx], threshold);
        time = System.currentTimeMillis() - time;

        mTimeView.setText("type:"+OpenCVHelper.CV_THRESHOLD_TYPE[typeIdx]+" threshold:"+threshold+" cost time:" + time);
        mDstImageView.setImageBitmap(mDstBitmap);
    }

    private static String typeString(int type) {
        switch (type){
            case OpenCVHelper.CV_THRESH_BINARY:
                return "CV_THRESH_BINARY";
            case OpenCVHelper.CV_THRESH_BINARY_INV:
                return "CV_THRESH_BINARY_INV";
            case OpenCVHelper.CV_THRESH_TRUNC:
                return "CV_THRESH_TRUNC";
            case OpenCVHelper.CV_THRESH_TOZERO:
                return "CV_THRESH_TOZERO";
            case OpenCVHelper.CV_THRESH_TOZERO_INV:
                return "CV_THRESH_TOZERO_INV";
            case OpenCVHelper.CV_THRESH_MASK:
                return "CV_THRESH_MASK";
            case OpenCVHelper.CV_THRESH_OTSU:
                return "CV_THRESH_OTSU";
        }
        return "";
    }
}
