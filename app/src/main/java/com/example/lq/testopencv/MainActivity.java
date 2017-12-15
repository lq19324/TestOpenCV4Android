package com.example.lq.testopencv;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ListView listView = (ListView)findViewById(R.id.mylist);
        final ArrayAdapter<ActivityInfo> adapter = new ArrayAdapter<ActivityInfo>(this, android.R.layout.simple_list_item_1, getMyDeclaredActivities(this));
        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ActivityInfo activityInfo = adapter.getItem(position);
                startActivity(activityInfo);
            }
        });

    }

    private void startActivity(ActivityInfo info) {
//        try {
            Intent intent = new Intent();
//            intent.setClass(MainActivity.this, Class.forName(info.name));
            intent.setClassName(info.packageName, info.name);
            startActivity(intent);
//        } catch (ClassNotFoundException e) {
//            e.printStackTrace();
//        }
    }

    private List<ActivityInfo> getMyDeclaredActivities(Context context) {
        PackageManager pm = context.getPackageManager();
        try {
            String thisPkgName = context.getApplicationInfo().packageName;
            PackageInfo thisPkgInfo = pm.getPackageInfo(thisPkgName, PackageManager.GET_ACTIVITIES);
            ArrayList<ActivityInfo>  activities = new ArrayList<>(Arrays.asList(thisPkgInfo.activities));
            for (ActivityInfo activ : activities) {
                if (activ.name.contains("MainActivity")) {
                    activities.remove(activ);
                    break;
                }
            }
            return activities;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }
}
