package com.polycom.polycom.broadcastReciver;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.polycom.polycom.R;

/**
 * Created by zsn on 2016/6/30.
 */
public class BroadCastActivity extends Activity {
    private NetworkChangeReceiver networkChangeReceiver;
    //本地广播
    private LocalBroadcastManager localBroadcastManager;
    private LocalReceiver localReceiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_broadcast);
        localBroadcastManager=LocalBroadcastManager.getInstance(this);
        //系统网络广播
        IntentFilter intentFilter=new IntentFilter();
        intentFilter.addAction("android.net.conn.CONNECTIVITY_CHANGE");
        networkChangeReceiver=new NetworkChangeReceiver();
        registerReceiver(networkChangeReceiver,intentFilter);
        //test git commit
        //..................................................

        //本地广播
        IntentFilter localIntentFilter=new IntentFilter();
        localIntentFilter.addAction("com.polycom.polycom.LOCAL_BROADCAST");
        localReceiver=new LocalReceiver();
        localBroadcastManager.registerReceiver(localReceiver, localIntentFilter);

        Button sendBroadcast = (Button) findViewById(R.id.send_broadcast);
        //自定义广播
        sendBroadcast.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Intent intent = new Intent("com.polycom.polycom.MY_BROADCAST");
                //com.polycom.polycom.LOCAL_BROADCAST
                Intent intent = new Intent("com.polycom.polycom.LOCAL_BROADCAST");
                intent.putExtra("data", "15311480854");
                //发送无序的广播
                //sendBroadcast(intent);
                //发送有序的广播
               // sendOrderedBroadcast(intent,null);
                //发送本地的广播
                localBroadcastManager.sendBroadcast(intent);
                //sendBroadcast(intent);
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(networkChangeReceiver);
        localBroadcastManager.unregisterReceiver(localReceiver);
    }

    class NetworkChangeReceiver extends BroadcastReceiver{

        @Override
        public void onReceive(Context context, Intent intent) {
            //Toast.makeText(BroadCastActivity.this,"network changed",1).show();
            ConnectivityManager connectivityManager=(ConnectivityManager)getSystemService(CONNECTIVITY_SERVICE);
            NetworkInfo networkInfo=connectivityManager.getActiveNetworkInfo();
            if(networkInfo!=null&&networkInfo.isAvailable()){
                Toast.makeText(context, "network is available",
                        Toast.LENGTH_SHORT).show();
            }else{
                Toast.makeText(context, "network is unavailable",
                        Toast.LENGTH_SHORT).show();
            }
        }
    }
}
