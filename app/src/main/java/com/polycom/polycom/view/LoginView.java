package com.polycom.polycom.view;

/**
 * Created by Administrator on 2016/6/13.
 */
public interface LoginView {
    void showProgress();
    void hideProgress();
    void navigateToHome();
    void setUsernameError();
    void setPasswordError();
}
