package com.polycom.polycom.model;

import android.content.SharedPreferences;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;

import com.polycom.polycom.OnLoginFinishListener;
import com.polycom.polycom.utils.AESUtils;
import com.polycom.polycom.utils.MD5Utils;

/**
 * 延时2秒登陆，如果名字或密码为空，则登陆失败，否则登陆成功
 * Created by zsn on 2016/6/13.
 */
public class LoginModelImpl implements LoginModel {
    // 注意：DES加密和解密过程中，密钥长度都必须是8的倍数
    private String key="zsnsdyma";
    //注意: AES加密和解密过程中,这里的password(秘钥必须是16位的)
    private static final String keyBytes = "abcdefghijklmn12";

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
                        //editor.putString("pwd", MD5Utils.getMD5String(pwd));
                        try {
                            //editor.putString("pwd", DesUtils.encrypt(pwd, key));
                            Log.i("eee","MD5加密的密码："+ MD5Utils.encode(pwd));
                            Log.i("eee","AES加密的密码："+AESUtils.encode(pwd,keyBytes));
                             editor.putString("pwd", AESUtils.encode(pwd, keyBytes));
                            editor.putBoolean("remeber_pwd", true);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
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
            try {
                String pwd = pref.getString("pwd", "");
                // pwd= DesUtils.decrypt(pwd, key);
                pwd=AESUtils.decode(pwd,keyBytes);
                onLoginFinishListener.onGetPwd(pwd);
                onLoginFinishListener.onGetCheckedButton(isRemeber);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
