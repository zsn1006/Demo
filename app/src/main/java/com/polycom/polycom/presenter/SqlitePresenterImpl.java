package com.polycom.polycom.presenter;

import android.database.sqlite.SQLiteDatabase;

import com.polycom.polycom.OnSqliteFinishListener;
import com.polycom.polycom.bean.SqlBean;
import com.polycom.polycom.model.SqliteModel;
import com.polycom.polycom.model.SqliteModelImpl;
import com.polycom.polycom.view.SqliteView;

import java.util.List;

/**
 * Created by zsn on 2016/7/8.
 */
public class SqlitePresenterImpl implements SqlitePresenter,OnSqliteFinishListener{
    private SqliteView sqliteView;
    private SqliteModel sqliteModel;
    public SqlitePresenterImpl(SqliteView sqliteView){
        this.sqliteView=sqliteView;
        this.sqliteModel =new SqliteModelImpl();
    }
    @Override
    public void insert(SQLiteDatabase db, String dbName,List<SqlBean> sqlList) {
        sqliteModel.insert(db,dbName,this,sqlList);
    }

    @Override
    public void update(SQLiteDatabase db, String dbName) {
        sqliteModel.update(db,dbName,this);
    }

    @Override
    public void delete(SQLiteDatabase db, String dbName) {
        sqliteModel.delete(db, dbName, this);
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

    @Override
    public void onUpdateSuccess() {
        if(sqliteView!=null){
            sqliteView.setUpdateSuccess();
        }
    }

    @Override
    public void onUpdateError() {
        if(sqliteView!=null){
            sqliteView.setUpdateError();
        }
    }

    @Override
    public void onDeleteSuccess() {
        if(sqliteView!=null){
            sqliteView.setDeleteSuccess();
        }
    }

    @Override
    public void onDeleteError() {
        if(sqliteView!=null){
            sqliteView.setDeleteError();
        }
    }
}
