package com.polycom.polycom.notification;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.polycom.polycom.R;
import com.polycom.polycom.utils.VersionUtils;

/**
 * Created by zsn on 2016/8/23.
 */
public class NotificationActivity extends Activity implements View.OnClickListener {
    private static final int NOTIFICATION_ID = 1000;
    private Button sendNoticeBtn;
    private int apiLevel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_notification);
        apiLevel=VersionUtils.getAPIVersion();
        String sdkVersion = VersionUtils.getSdkVersion();

        Log.i("zsn","apiLevel==="+apiLevel);
        Log.i("zsn","sdkVersion==="+sdkVersion);
        Log.i("zsn","Version==="+ "Product Model: " + android.os.Build.MODEL + ","
                + Build.VERSION.SDK_INT + ","
                + android.os.Build.VERSION.RELEASE);
        initView();
    }

    private void initView() {
        sendNoticeBtn=(Button)findViewById(R.id.send_notice);
        sendNoticeBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.send_notice:
                NotificationManager notificationManager= (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
                sendNotification(notificationManager,apiLevel);
                break;
        }
    }

    /**
     * 发送通知 alt+shift+m
     * @param notificationManager
     */
    private void sendNotification(NotificationManager notificationManager,int api) {
        //api<11
        if(api< Build.VERSION_CODES.HONEYCOMB){
            notificationForAPIBelow11(notificationManager);
        }else{
            notificationForAPI11with16(notificationManager,api);
        }

    }

    /**
     * api<11
     * @param notificationManager
     */
    private void notificationForAPIBelow11(NotificationManager notificationManager) {
        Notification notification = new Notification(
                R.mipmap.ic_launcher, "这是api<11通知的标题", System.currentTimeMillis());
        notification.defaults = 0;

        Uri uri = Uri.parse("http://baidu.com");
        Intent intent = new Intent(Intent.ACTION_VIEW, uri);
        intent.setComponent(null);

        PendingIntent pi = PendingIntent.getActivity(
                getApplicationContext(), 0, intent, 0);
        notification.setLatestEventInfo(getApplicationContext(), "这是通知的标题",
                "这是内容", pi);
        notificationManager.notify(1000, notification);
    }

    /**
     * 区别api（11 and 16）
     * @param notificationManager
     * @param api
     */
    private void notificationForAPI11with16(NotificationManager notificationManager,int api) {
        Notification notification;
        NotificationCompat.Builder build=new NotificationCompat.Builder(getApplicationContext());
        Uri uri=Uri.parse("http://baidu.com");
        Intent i = new Intent(Intent.ACTION_VIEW,uri);
        i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        i.setComponent(null);
        //该语句的作用是定义了一个不是当即显示的activity，只有当用户拉下notify显示列表，并且单击对应的项的时候，才会触发系统跳转到该activity.
        PendingIntent intent = PendingIntent.getActivity(getApplicationContext(), 0,i,PendingIntent.FLAG_CANCEL_CURRENT);
        //PendingIntent点击通知后所跳转的页面
        build.setContentInfo("补充内容");
        build.setContentTitle("标题");
        build.setContentText(" 新消息,点击查看。");
        build.setSmallIcon(R.mipmap.ic_launcher);
        build.setTicker("新消息");
        build.setWhen(System.currentTimeMillis());// 通知产生的时间，会在通知信息里显示，一般是系统获取到的时间
        build.setPriority(NotificationCompat.PRIORITY_MAX); // 设置该通知优先级
        build.setVisibility(NotificationCompat.VISIBILITY_PUBLIC);//在任何情况下都显示，不受锁屏影响。
        build.setAutoCancel(true);// 设置这个标志当用户单击面板就可以让通知将自动取消  

        build.setOngoing(false);// true，设置他为一个正在进行的通知。他们通常是用来表示一个后台任务,用户积极参与(如播放音乐)或以某种方式正在等待,因此占用设备(如一个文件下载,同步操作,主动网络连接)
        // 向通知添加声音、闪灯和振动效果的最简单、最一致的方式是使用当前的用户默认设置，使用NotificationCompat.DEFAULT_ALL属性，可以组合  
        build.setVibrate(new long[]{0, 100, 500, 100});//振动效果需要振动权限

        build.setSound(Uri.parse("android.resource://" + getPackageName()//声音  
                + "/" + R.raw.incomingcall));

        build.setDefaults(NotificationCompat.DEFAULT_SOUND); //声音
        build.setDefaults(NotificationCompat.DEFAULT_LIGHTS); //指示灯
        build.setDefaults(NotificationCompat.DEFAULT_VIBRATE); //震动

        // build.setDefaults(NotificationCompat.DEFAULT_ALL);
        //执行intent
        build.setContentIntent(intent);
        if(api<Build.VERSION_CODES.JELLY_BEAN){
            notification=build.getNotification();
        }else{
            notification=build.build();
        }
        notificationManager.notify(NOTIFICATION_ID,notification);
    }
}
