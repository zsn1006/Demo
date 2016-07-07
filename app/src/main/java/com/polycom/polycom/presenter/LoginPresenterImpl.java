package com.polycom.polycom.presenter;

import android.content.SharedPreferences;

import com.polycom.polycom.OnLoginFinishListener;
import com.polycom.polycom.model.LoginModel;
import com.polycom.polycom.model.LoginModelImpl;
import com.polycom.polycom.view.LoginView;

/**
 * Created by Administrator on 2016/6/13.
 */
public class LoginPresenterImpl implements LoginPresenter,OnLoginFinishListener {
    private LoginView loginView;
    private LoginModel loginModel;
   public LoginPresenterImpl(LoginView loginView){
       this.loginView=loginView;
       this.loginModel=new LoginModelImpl();
   }

    @Override
    public void validateCredentials(String userName, String pwd, boolean checked,SharedPreferences pref) {
        if(loginView!=null){
            loginView.showProgress();
        }
        loginModel.login(userName,pwd,checked,pref,this);
    }

    @Override
    public void onDestroy() {
        loginView=null;
    }

    @Override
    public void getSP(SharedPreferences pref) {
        loginModel.getSPData(pref,this);
    }

    @Override
    public void onUserNameError() {
        if(loginView!=null){
            loginView.setUsernameError();
            loginView.hideProgress();
        }
    }

    @Override
    public void onPwdError() {
        if(loginView!=null){
            loginView.setPasswordError();
            loginView.hideProgress();
        }
    }

    @Override
    public void onSuccess() {
        if(loginView!=null){
            loginView.hideProgress();
            loginView.startNewActivity();
        }
    }

    @Override
    public void onGetUserName(String name) {
        if(loginView!=null){
            loginView.setCheckedUserName(name);
        }
    }

    @Override
    public void onGetPwd(String pwd) {
        if(loginView!=null){
            loginView.setCheckedPassword(pwd);
        }
    }

    @Override
    public void onGetCheckedButton(boolean checkedBtn) {
        if(loginView!=null){
            loginView.setCheckedButton(checkedBtn);
        }
    }

}
