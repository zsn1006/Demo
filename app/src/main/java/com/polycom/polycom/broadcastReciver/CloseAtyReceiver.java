package com.polycom.polycom.broadcastReciver;

import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

import com.polycom.polycom.LoginActivity;
import com.polycom.polycom.R;
import com.polycom.polycom.utils.ActivityCollector;

/**
 * Created by zsn on 2016/7/5.
 */
public class CloseAtyReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(final Context context, final Intent intent) {
        AlertDialog.Builder builder=new AlertDialog.Builder(context);
        View title = View.inflate(context, R.layout.title, null);
        builder.setCustomTitle(title)
                .setMessage("关闭所有界面哦？")
                .setCancelable(false)
                .setPositiveButton("确认", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        //销毁所有的活动
                        ActivityCollector.finishAll();
                        dialog.dismiss();
                        Log.i("zsn","确认 which="+which);
                        Intent i=new Intent(context, LoginActivity.class);
                        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        context.startActivity(i);
                    }
                }).setNegativeButton("取消", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                Log.i("zsn", "取消 which=" + which);
            }
        });
        AlertDialog alertDialog=builder.create();
        alertDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        alertDialog.show();
    }
}
