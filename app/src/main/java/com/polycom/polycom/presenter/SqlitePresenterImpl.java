package com.polycom.polycom.presenter;

import android.database.sqlite.SQLiteDatabase;

import com.polycom.polycom.OnSqliteFinishListener;
import com.polycom.polycom.model.SqliteModel;
import com.polycom.polycom.model.SqliteModelImpl;
import com.polycom.polycom.view.SqliteView;

/**
 * Created by zsn on 2016/7/8.
 */
public class SqlitePresenterImpl implements SqlitePresenter,OnSqliteFinishListener{
    private SqliteView sqliteView;
    private SqliteModel sqliteModel;
    public SqlitePresenterImpl(SqliteView sqliteView){
        this.sqliteView=sqliteView;
        this.sqliteModel=new SqliteModelImpl();
    }
    @Override
    public void insert(SQLiteDatabase db, String dbName) {
        sqliteModel.insert(db,dbName,this);
    }

    @Override
    public void onDestroy() {
        sqliteView=null;
    }

    @Override
    public void onInsertSuccess() {
        if(sqliteView!=null){
            sqliteView.setInsertSuccess();
        }
    }

    @Override
    public void onInsertError() {
        if(sqliteView!=null){
            sqliteView.setInsertError();
        }
    }
}
