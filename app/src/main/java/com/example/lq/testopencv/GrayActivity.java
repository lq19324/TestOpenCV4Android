package com.example.lq.testopencv;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.IOException;
/**
 * Created by lq on 17-11-30.
 */

public class GrayActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_gray);

        AssetManager am = getAssets();
        try {
            Bitmap src = BitmapFactory.decodeStream(am.open("test3.jpg"));
            Bitmap dst = Bitmap.createBitmap(src.getWidth(), src.getHeight(), Bitmap.Config.ARGB_8888);

            long time = System.currentTimeMillis();
            OpenCVHelper.nativeProcessImage(src, dst, 1<<8);
            time = System.currentTimeMillis() - time;

            ((TextView) findViewById(R.id.tv)).setText("cost time:" + time + " ms");

            ImageView view = (ImageView)findViewById(R.id.src_iv);
            view.setImageBitmap(src);

            ImageView dst_view = (ImageView)findViewById(R.id.dst_iv);
            dst_view.setImageBitmap(dst);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
