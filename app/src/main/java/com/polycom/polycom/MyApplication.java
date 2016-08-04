package com.polycom.polycom;

import android.app.Application;

import com.socks.library.KLog;

/**
 * Created by Administrator on 2016/5/27.
 */
public class MyApplication extends Application {
    private static final String TAG="application";

    @Override
    public void onCreate() {
        super.onCreate();
        KLog.init(BuildConfig.LOG_DEBUG, "Zsn");
    }
}
