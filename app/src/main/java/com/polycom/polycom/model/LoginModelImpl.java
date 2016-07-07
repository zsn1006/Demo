package com.polycom.polycom.model;

import android.content.SharedPreferences;
import android.os.Handler;
import android.text.TextUtils;

import com.polycom.polycom.OnLoginFinishListener;

/**
 * 延时2秒登陆，如果名字或密码为空，则登陆失败，否则登陆成功
 * Created by zsn on 2016/6/13.
 */
public class LoginModelImpl implements LoginModel {

    @Override
    public void login(final String userName, final String pwd, final boolean checked,final SharedPreferences pref,final OnLoginFinishListener onLoginFinishListener) {
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                boolean error=false;
                if(TextUtils.isEmpty(userName)){
                    onLoginFinishListener.onUserNameError();
                    error=true;
                }
                if(TextUtils.isEmpty(pwd)){
                    onLoginFinishListener.onPwdError();
                    error=true;
                }

                if(!error){
                    onLoginFinishListener.onSuccess();
                    SharedPreferences.Editor editor=pref.edit();
                    editor.putString("name",userName);
                    if(checked){
                        editor.putString("pwd", pwd);
                        editor.putBoolean("remeber_pwd", true);
                    }else{
                        editor.putString("pwd", "");
                        editor.putBoolean("remeber_pwd", false);
                    }
                    editor.commit();
                }
            }
        },2000);
    }

    @Override
    public void getSPData(SharedPreferences pref,OnLoginFinishListener onLoginFinishListener) {
        boolean isRemeber = pref.getBoolean("remeber_pwd", false);
        String name = pref.getString("name", "");
        onLoginFinishListener.onGetUserName(name);
        if(isRemeber) {
            String pwd = pref.getString("pwd", "");
            onLoginFinishListener.onGetPwd(pwd);
            onLoginFinishListener.onGetCheckedButton(isRemeber);
        }
    }
}
