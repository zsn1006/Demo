package com.polycom.polycom.model;

import android.database.sqlite.SQLiteDatabase;

import com.polycom.polycom.OnSqliteFinishListener;
import com.polycom.polycom.bean.SqlBean;

import java.util.List;

/**
 * Created by zsn on 2016/7/8.
 */
public interface SqliteModel {
    void insert(SQLiteDatabase db,String tableName,OnSqliteFinishListener onSqliteFinishListener,List<SqlBean> sqlList);
    void update(SQLiteDatabase db,String tableName,OnSqliteFinishListener onSqliteFinishListener);
    void delete(SQLiteDatabase db, String dbName, OnSqliteFinishListener onSqliteFinishListener);
    void query(SQLiteDatabase db, String dbName, OnSqliteFinishListener onSqliteFinishListener);
    void replaceData(SQLiteDatabase db, String dbName, OnSqliteFinishListener onSqliteFinishListener);
}
