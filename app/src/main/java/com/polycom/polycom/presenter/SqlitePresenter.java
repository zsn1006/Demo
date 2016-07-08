package com.polycom.polycom.presenter;

import android.database.sqlite.SQLiteDatabase;

/**
 *
 * Created by zsn on 2016/7/8.
 */
public interface SqlitePresenter {
    void insert(SQLiteDatabase db,String dbName);
    void onDestroy();
}
