package com.polycom.polycom;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.polycom.polycom.utils.ActivityCollector;

/**
 * 活动的基类
 * Created by zsn on 2016/7/5.
 */
public class BaseActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityCollector.addActivity(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ActivityCollector.removeActivity(this);
    }

    public void startAty(Class<?>cls){
        Intent intent=new Intent(this,cls);
        startActivity(intent);
    }
}
