package com.example.lq.testopencv;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

import java.io.IOException;
/**
 * Created by lq on 17-11-30.
 */

public class BlurActivity extends Activity {

    private Bitmap mSrcBitmap;
    private Bitmap mDstBitmap;

    private ImageView mSrcImageView;
    private ImageView mDstImageView;

    private TextView mTimeView;

    private SeekBar mBlurLevelSeekBar;

    private int mBlurLevel = 3;
    private int mBlurType = 2;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_blur);

        mSrcImageView = (ImageView)findViewById(R.id.src_iv);
        mDstImageView = (ImageView)findViewById(R.id.dst_iv);
        mTimeView = (TextView)findViewById(R.id.tv_time);
        mBlurLevelSeekBar = (SeekBar)findViewById(R.id.seek_blur_level);
        mBlurLevelSeekBar.setOnSeekBarChangeListener(mSeekListener);

        AssetManager am = getAssets();
        try {
            mSrcBitmap = BitmapFactory.decodeStream(am.open("test3.jpg"));
            mDstBitmap = Bitmap.createBitmap(mSrcBitmap.getWidth(), mSrcBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        } catch (IOException e) {
            e.printStackTrace();
        }
        mSrcImageView.setImageBitmap(mSrcBitmap);
    }

    private SeekBar.OnSeekBarChangeListener mSeekListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            int blurLevel = seekBar.getProgress();
            mBlurLevel = ((blurLevel >> 1) << 1) + 1;
            int value = (mBlurType << 8) | mBlurLevel;
            long time = System.currentTimeMillis();
            OpenCVHelper.nativeProcessImage(mSrcBitmap, mDstBitmap, value);
            mDstImageView.setImageBitmap(mDstBitmap);
            mTimeView.setText("type:"+mBlurType+" level:"+mBlurLevel+"-"+blurLevel+" cost time: "+ (System.currentTimeMillis() - time) +" ms");
        }
    };

    public void onButtonClick(View view) {
        int viewID = view.getId();
        long time = System.currentTimeMillis();
        if (viewID == R.id.but_blur) {
            mBlurType = 2;
            int value = (mBlurType << 8) | mBlurLevel;
            OpenCVHelper.nativeProcessImage(mSrcBitmap, mDstBitmap, value);
        } else if (viewID == R.id.but_medium_blur) {
            mBlurType = 3;
            int value = (mBlurType << 8) | mBlurLevel;
            OpenCVHelper.nativeProcessImage(mSrcBitmap, mDstBitmap, value);
        } else if (viewID == R.id.but_gaussian_blur) {
            mBlurType = 4;
            int value = (mBlurType << 8) | mBlurLevel;
            OpenCVHelper.nativeProcessImage(mSrcBitmap, mDstBitmap, value);
        }
        mDstImageView.setImageBitmap(mDstBitmap);
        mTimeView.setText("type:"+mBlurType+" level:"+mBlurLevel+" cost time: "+ (System.currentTimeMillis() - time) +" ms");
    }
}
