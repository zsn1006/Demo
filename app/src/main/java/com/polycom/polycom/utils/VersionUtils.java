package com.polycom.polycom.utils;

import android.os.Build;

/**
 * Created by zsn on 2016/8/23.
 */
public class VersionUtils {
    /**
     * 判断Android系统API的版本
     * @return
     */
    public static int getAPIVersion(){
        int APIVersion;
        try{
            APIVersion=Integer.valueOf(Build.VERSION.SDK_INT);
        }catch (Exception e){
            APIVersion=0;

        }
        return APIVersion;
    }
    /**
     * 判断Android系统SDK的版本，我们一般用currentVersion < android.os.Build.VERSION_CODES.FROYO的方式进行判断是2.2以下版本
     *
     * @return
     */
    public static String getSdkVersion(){
        String sdkVersion;
        try{
            sdkVersion = Build.VERSION.RELEASE;
        }catch (Exception e){
            sdkVersion="";

        }
        return sdkVersion;
    }

}
