package com.polycom.polycom;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ProgressBar;

import com.polycom.polycom.presenter.LoginPresenter;
import com.polycom.polycom.presenter.LoginPresenterImpl;
import com.polycom.polycom.view.LoginView;

/**
 * MVP模式
 *
 * Created by zsn on 2016/6/13.
 * changed by zsn on 2016/7/7
 */
public class LoginActivity extends BaseActivity implements LoginView,View.OnClickListener {
    private ProgressBar progressBar;
    private EditText username;
    private EditText password;
    private LoginPresenter presenter;
    private CheckBox remeberPWD;
    private SharedPreferences pref;
    private Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        initView();
        initListener();
        initData();
    }

    private void initView() {
        progressBar = (ProgressBar) findViewById(R.id.progress);
        username = (EditText) findViewById(R.id.username);
        password = (EditText) findViewById(R.id.password);
        button=(Button)findViewById(R.id.button);
        remeberPWD = (CheckBox) findViewById(R.id.remeber_pass);
    }

    private void initListener() {
        remeberPWD.setOnClickListener(this);
        button.setOnClickListener(this);
    }

    private void initData() {
        pref= PreferenceManager.getDefaultSharedPreferences(this);
        presenter=new LoginPresenterImpl(this);
        presenter.getSP(pref);
    }
    @Override
    protected void onDestroy() {
        presenter.onDestroy();
        super.onDestroy();
    }

    @Override
    public void showProgress() {
        progressBar.setVisibility(View.VISIBLE);
    }

    @Override
    public void hideProgress() {
        progressBar.setVisibility(View.GONE);
    }

    @Override
    public void setUsernameError() {
        username.setError(getString(R.string.username_error));
    }

    @Override
    public void setPasswordError() {
        password.setError(getString(R.string.password_error));
    }

    @Override
    public void setCheckedUserName(String name) {
        username.setText(name);
    }

    @Override
    public void setCheckedPassword(String pwd) {
        password.setText(pwd);
    }

    @Override
    public void setCheckedButton(boolean checkedBtn) {
        remeberPWD.setChecked(checkedBtn);
    }

    @Override
    public void startNewActivity() {
       startActivity(new Intent(this,MainActivity.class));
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.button:
                presenter.validateCredentials(username.getText().toString(),password.getText().toString(),remeberPWD.isChecked(),pref);
                break;
            default:
                break;
        }

    }
}
