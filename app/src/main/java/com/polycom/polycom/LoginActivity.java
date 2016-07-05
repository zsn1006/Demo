package com.polycom.polycom;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.ProgressBar;

import com.polycom.polycom.presenter.LoginPresenter;
import com.polycom.polycom.presenter.LoginPresenterImpl;
import com.polycom.polycom.view.LoginView;

/**
 * Created by Administrator on 2016/6/13.
 */
public class LoginActivity extends BaseActivity implements LoginView,View.OnClickListener {
    private ProgressBar progressBar;
    private EditText username;
    private EditText password;
    private LoginPresenter presenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        //test
        progressBar = (ProgressBar) findViewById(R.id.progress);
        username = (EditText) findViewById(R.id.username);
        password = (EditText) findViewById(R.id.password);
        findViewById(R.id.button).setOnClickListener(this);

        presenter=new LoginPresenterImpl(this);
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
    public void startNewActivity() {
       startActivity(new Intent(this,MainActivity.class));
    }

    @Override
    public void onClick(View v) {
        presenter.validateCredentials(username.getText().toString(),password.getText().toString());
    }
}
