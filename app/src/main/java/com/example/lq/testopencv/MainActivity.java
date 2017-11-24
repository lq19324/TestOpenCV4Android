package com.example.lq.testopencv;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.widget.ImageView;

import java.io.IOException;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AssetManager am = getAssets();
        try {
            Bitmap src = BitmapFactory.decodeStream(am.open("test3.jpg"));
            Bitmap dst = Bitmap.createBitmap(src.getWidth(), src.getHeight(), Bitmap.Config.ARGB_8888);

            OpenCVHelper.grayImage(src, dst);

            ImageView view = (ImageView)findViewById(R.id.src_iv);
            view.setImageBitmap(src);

            ImageView dst_view = (ImageView)findViewById(R.id.dst_iv);
            dst_view.setImageBitmap(dst);
        } catch (IOException e) {
            e.printStackTrace();
        }


    }
}
