package com.polycom.polycom.view;

/**
 * Created by zsn on 2016/6/13.
 */
public interface LoginView {
    void showProgress();
    void hideProgress();
    void setUsernameError();
    void setPasswordError();
    void setCheckedUserName(String name);
    void setCheckedPassword(String pwd);
    void setCheckedButton(boolean checkedBtn);
    void startNewActivity();
}
