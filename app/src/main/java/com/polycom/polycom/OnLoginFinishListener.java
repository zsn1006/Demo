package com.polycom.polycom;

/**
 *登陆事件的监听
 * Created by zsn on 2016/6/13.
 */
public interface OnLoginFinishListener {
    void onUserNameError();
    void onPwdError();
    void onSuccess();
    void onGetUserName(String name);
    void onGetPwd(String pwd);
    void onGetCheckedButton(boolean checkedBtn);
}
