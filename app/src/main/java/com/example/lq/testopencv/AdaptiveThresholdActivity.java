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

public class AdaptiveThresholdActivity extends Activity {

    private Bitmap mSrcBitmap;
    private Bitmap mDstBitmap;

    private ImageView mSrcImageView;
    private ImageView mDstImageView;

    private TextView mTimeView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_adaptive_threshold);

        mSrcImageView = (ImageView)findViewById(R.id.src_iv);
        mDstImageView = (ImageView)findViewById(R.id.dst_iv);
        mTimeView = (TextView)findViewById(R.id.tv_time);

        SeekBar paramSeekBar = (SeekBar)findViewById(R.id.sb_param);
        paramSeekBar.setProgress(55);
        paramSeekBar.setOnSeekBarChangeListener(mSeekListener);

        SeekBar blockSizeSeekBar = (SeekBar)findViewById(R.id.sb_blockSize);
        blockSizeSeekBar.setOnSeekBarChangeListener(mSeekListener);

        AssetManager am = getAssets();
        try {
            mSrcBitmap = BitmapFactory.decodeStream(am.open("threshold1.jpg"));
            mDstBitmap = Bitmap.createBitmap(mSrcBitmap.getWidth(), mSrcBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        } catch (IOException e) {
            e.printStackTrace();
        }
        mSrcImageView.setImageBitmap(mSrcBitmap);
    }

    private int blockSize = 3;
    private int param = 5;
    private SeekBar.OnSeekBarChangeListener mSeekListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            if (seekBar.getId() == R.id.sb_param) {
                int progress = seekBar.getProgress();
                param = progress - 50;
                mTimeView.setText("blockSize:"+blockSize+" param:"+param);
            } else if (seekBar.getId() == R.id.sb_blockSize) {
                int progress = seekBar.getProgress();
                blockSize = ((progress >> 1) << 1) + 1;
                mTimeView.setText("blockSize:"+blockSize+" param:"+param);
            }
        }
    };

    private int method = OpenCVHelper.CV_ADAPTIVE_THRESH_MEAN_C;
    private int type = OpenCVHelper.CV_THRESH_BINARY;
    public void onButtonClick(View view) {
        int viewID = view.getId();
        if (viewID == R.id.but_type) {
            type++;
            type%=2;
            if (view instanceof Button) {
                ((Button)view).setText("type="+typeString(type));
            }
        } if (viewID == R.id.but_method){
            method++;
            method%=2;
            if (view instanceof Button) {
                ((Button)view).setText("method="+methodString(method));
            }
        } else if (viewID == R.id.but_apply) {
            processImage();
        }
    }

    private void processImage() {
        Canvas canvas = new Canvas(mDstBitmap);
        canvas.drawBitmap(mSrcBitmap, 0, 0, null);

        long time = System.currentTimeMillis();
        OpenCVHelper.nativeAdaptiveThreshold(mDstBitmap, method, type, blockSize, param);
        time = System.currentTimeMillis() - time;

        mTimeView.setText("blockSize:"+blockSize+" param:"+param+" cost time:" + time);
        mDstImageView.setImageBitmap(mDstBitmap);
    }

    private static String methodString(int type) {
        switch (type){
            case OpenCVHelper.CV_ADAPTIVE_THRESH_MEAN_C:
                return "THRESH_MEAN_C";
            case OpenCVHelper.CV_ADAPTIVE_THRESH_GAUSSIAN_C:
                return "THRESH_GAUSSIAN_C";
        }
        return "";
    }

    private static String typeString(int type) {
        switch (type){
            case OpenCVHelper.CV_THRESH_BINARY:
                return "BINARY";
            case OpenCVHelper.CV_THRESH_BINARY_INV:
                return "BINARY_INV";
        }
        return "";
    }
}
