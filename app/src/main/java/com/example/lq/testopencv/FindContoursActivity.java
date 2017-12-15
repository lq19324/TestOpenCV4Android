package com.example.lq.testopencv;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Point;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.IOException;
import java.util.ArrayList;
/**
 * Created by lq on 17-11-30.
 */

public class FindContoursActivity extends Activity {

    private Bitmap mSrcBitmap;
    private Bitmap mDstBitmap;

    private ImageView mSrcImageView;
    private ImageView mDstImageView;

    private TextView mTimeView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_find_contours);

        mSrcImageView = (ImageView)findViewById(R.id.src_iv);
        mDstImageView = (ImageView)findViewById(R.id.dst_iv);
        mTimeView = (TextView)findViewById(R.id.tv_time);

        AssetManager am = getAssets();
        try {
            mSrcBitmap = BitmapFactory.decodeStream(am.open("test_find_contours.jpg"));
            mDstBitmap = Bitmap.createBitmap(mSrcBitmap.getWidth(), mSrcBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        } catch (IOException e) {
            e.printStackTrace();
        }
        mSrcImageView.setImageBitmap(mSrcBitmap);
    }

    private int mode = OpenCVHelper.CV_RETR_CCOMP;
    private int method = OpenCVHelper.CV_CHAIN_APPROX_NONE;
    public void onButtonClick(View view) {
        int viewID = view.getId();
        if (viewID == R.id.but_mode) {
            mode%=5;
            if (view instanceof Button) {
                ((Button)view).setText("mode="+mode);
            }
            mode++;
        } else if (viewID == R.id.but_method) {
            method%=6;
            if (view instanceof Button) {
                ((Button)view).setText("method="+method);
            }
            method++;
        } else if (viewID == R.id.but_apply) {
            processImage();
        }
    }

    private void processImage() {
        ArrayList<ArrayList<Point>> contours = new ArrayList<>();
        long time = System.currentTimeMillis();
        OpenCVHelper.nativeFindContours(mSrcBitmap, mode, method, contours);
        time = System.currentTimeMillis() - time;

        Canvas canvas = new Canvas(mDstBitmap);
        canvas.drawBitmap(mSrcBitmap, 0, 0, null);

        int[] colors = {Color.RED, Color.BLUE, Color.CYAN, Color.GREEN, Color.LTGRAY};

        Paint paint = new Paint();
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(6);

        int size = contours.size();
        for (int i = 0; i < size; i++) {
            paint.setColor(colors[i%5]);
            Path ai = new Path();
            int aSize = contours.get(i).size();
            for (int j = 0; j < aSize; j++) {
                if (j == 0) {
                    ai.moveTo(contours.get(i).get(j).x, contours.get(i).get(j).y);
                } else {
                    ai.lineTo(contours.get(i).get(j).x, contours.get(i).get(j).y);
                }
            }
            canvas.drawPath(ai, paint);
        }
        mTimeView.setText("cost time:" + time + " ms contour size:" + size);
        mDstImageView.setImageBitmap(mDstBitmap);
    }
}
