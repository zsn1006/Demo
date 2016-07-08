package com.polycom.polycom;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.polycom.polycom.presenter.SqlitePresenter;
import com.polycom.polycom.presenter.SqlitePresenterImpl;
import com.polycom.polycom.sqlite.MyDataBaseHelper;
import com.polycom.polycom.utils.ToastUtil;
import com.polycom.polycom.view.SqliteView;

/**
 * Created by Administrator on 2016/7/8.
 */
public class MySqliteActivity extends BaseActivity implements SqliteView{

    private Button sqliteBtn;
    private MyDataBaseHelper myDataBaseHelper;
    private SqlitePresenter sqlitePresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sqlite);
        initView();
        initData();
        initListener();
    }

    private void initListener() {
        sqliteBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sqlitePresenter.insert(myDataBaseHelper.getWritableDatabase(),MyDataBaseHelper.TABLE_NAME_1);
            }
        });
    }

    private void initData() {
        myDataBaseHelper=new MyDataBaseHelper(this,"BookStore.db",null,2);
        sqlitePresenter=new SqlitePresenterImpl(this);
    }

    private void initView() {
        sqliteBtn=(Button)findViewById(R.id.sqlite_btn);
    }


    @Override
    public void setInsertSuccess() {
        ToastUtil.showToast(this,"插入成功");
    }

    @Override
    public void setInsertError() {
        ToastUtil.showToast(this,"插入失败");
    }
}
