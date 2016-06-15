package com.polycom.polycom.model;

import com.polycom.polycom.OnLoginFinishListener;

/**
 * Created by Administrator on 2016/6/13.
 */
public interface LoginModel {
    void login(String userName,String pwd,OnLoginFinishListener onLoginFinishListener);
}
