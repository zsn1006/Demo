package com.polycom.polycom.presenter;

/**
 * 完成登陆的验证，以及销毁当前view
 * Created by Administrator on 2016/6/13.
 */
public interface LoginPresenter {
    void validateCredentials(String userName,String pwd);
    void onDestroy();
}
