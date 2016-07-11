package com.polycom.polycom;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.polycom.polycom.bean.SqlBean;
import com.polycom.polycom.presenter.SqlitePresenter;
import com.polycom.polycom.presenter.SqlitePresenterImpl;
import com.polycom.polycom.sqlite.MyDataBaseHelper;
import com.polycom.polycom.utils.ToastUtil;
import com.polycom.polycom.view.SqliteView;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Administrator on 2016/7/8.
 */
public class MySqliteActivity extends BaseActivity implements SqliteView{

    private Button sqliteInsertBtn,sqliteUpdateBtn,sqliteDeleteBtn;
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
        sqliteInsertBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                List<SqlBean>sqlList=new ArrayList<SqlBean>();
                for(int i=0;i<3;i++){
                    SqlBean sqlBean=new SqlBean();
                    sqlBean.setAuthor("小智");
                    sqlBean.setName("无名氏");
                    sqlBean.setPages(100);
                    sqlBean.setPrice(10.06);
                    sqlList.add(sqlBean);
                }
                sqlitePresenter.insert(myDataBaseHelper.getWritableDatabase(),MyDataBaseHelper.TABLE_NAME_1,sqlList);
            }
        });
        sqliteUpdateBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sqlitePresenter.update(myDataBaseHelper.getWritableDatabase(), MyDataBaseHelper.TABLE_NAME_1);
            }
        });
        sqliteDeleteBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sqlitePresenter.delete(myDataBaseHelper.getWritableDatabase(), MyDataBaseHelper.TABLE_NAME_1);
            }
        });
    }

    private void initData() {
        myDataBaseHelper=new MyDataBaseHelper(this,"BookStore.db",null,2);
        sqlitePresenter=new SqlitePresenterImpl(this);
    }

    private void initView() {
        sqliteInsertBtn=(Button)findViewById(R.id.sqlite_insert_btn);
        sqliteUpdateBtn=(Button)findViewById(R.id.sqlite_update_btn);
        sqliteDeleteBtn=(Button)findViewById(R.id.sqlite_delete_btn);
    }


    @Override
    public void setInsertSuccess() {
        ToastUtil.showToast(this,"插入成功");
    }

    @Override
    public void setInsertError() {
        ToastUtil.showToast(this,"插入失败");
    }

    @Override
    public void setUpdateSuccess() {
        ToastUtil.showToast(this,"更新成功");
    }

    @Override
    public void setUpdateError() {
        ToastUtil.showToast(this,"更新失败");
    }

    @Override
    public void setDeleteSuccess() {
        ToastUtil.showToast(this,"删除成功");
    }

    @Override
    public void setDeleteError() {
        ToastUtil.showToast(this,"删除失败");
    }
}
