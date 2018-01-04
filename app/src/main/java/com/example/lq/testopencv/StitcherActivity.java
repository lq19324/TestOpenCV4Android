package com.example.lq.testopencv;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.IOException;
/**
 * Created by lq on 18-1-3.
 */

public class StitcherActivity extends Activity{

    private Bitmap stitcherImage1;
    private Bitmap stitcherImage2;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_stitcher);

        AssetManager am = getAssets();
        try {
            stitcherImage1 = BitmapFactory.decodeStream(am.open("testStitcher2.jpg"));
            stitcherImage2 = BitmapFactory.decodeStream(am.open("testStitcher1.jpg"));

            ImageView view = (ImageView)findViewById(R.id.img1);
            view.setImageBitmap(stitcherImage1);

            ImageView dst_view = (ImageView)findViewById(R.id.img2);
            dst_view.setImageBitmap(stitcherImage2);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void onButtonClick(View view) {
        // TODO stitcher images

        long time = System.currentTimeMillis();
        OpenCVHelper.nativeStitcher(stitcherImage1, stitcherImage2, null);
        Toast.makeText(this, "pay time:"+(System.currentTimeMillis() - time), Toast.LENGTH_SHORT).show();
    }
}
