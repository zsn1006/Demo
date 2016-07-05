package com.polycom.polycom.utils;

import android.app.Activity;

import java.util.ArrayList;
import java.util.List;

/**
 * 活动管理类
 * Created by zsn on 2016/7/5.
 */
public class ActivityCollector {
    public static List<Activity>activities=new ArrayList<>();
    public static void addActivity(Activity activity){
        activities.add(activity);
    }
    public static void removeActivity(Activity activity){
        activities.remove(activity);
    }

    public static void finishAll(){
        for(Activity activity:activities){
            if(!activity.isFinishing()){
                activity.finish();
            }
        }
    }
}
