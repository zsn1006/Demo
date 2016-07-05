package com.polycom.polycom.model;

import android.os.Handler;
import android.text.TextUtils;

import com.polycom.polycom.OnLoginFinishListener;

/**
 * 延时2秒登陆，如果名字或密码为空，则登陆失败，否则登陆成功
 * Created by Administrator on 2016/6/13.
 */
public class LoginModelImpl implements LoginModel {
    @Override
    public void login(final String userName, final String pwd, final OnLoginFinishListener onLoginFinishListener) {
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

                }
            }
        },2000);
    }

}
