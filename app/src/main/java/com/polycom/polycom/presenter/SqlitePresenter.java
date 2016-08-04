package com.polycom.polycom.presenter;

import android.database.sqlite.SQLiteDatabase;

import com.polycom.polycom.bean.SqlBean;

import java.util.List;

/**
 *
 * Created by zsn on 2016/7/8.
 */
public interface SqlitePresenter {
    void insert(SQLiteDatabase db,String dbName,List<SqlBean> sqlList);
    void update(SQLiteDatabase db,String dbName);
    void delete(SQLiteDatabase db, String dbName);
    void query(SQLiteDatabase db, String dbName);
    void replaceData(SQLiteDatabase db, String dbName);
    void onDestroy();
}
