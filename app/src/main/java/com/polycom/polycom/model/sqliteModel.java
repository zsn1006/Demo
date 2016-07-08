package com.polycom.polycom.model;

import android.database.sqlite.SQLiteDatabase;

import com.polycom.polycom.OnSqliteFinishListener;

/**
 * Created by zsn on 2016/7/8.
 */
public interface SqliteModel {
    void insert(SQLiteDatabase db,String tableName,OnSqliteFinishListener onSqliteFinishListener);
}
