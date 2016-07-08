package com.polycom.polycom.model;

import android.content.ContentValues;
import android.database.sqlite.SQLiteDatabase;

import com.polycom.polycom.OnSqliteFinishListener;

/**
 * Created by zsn on 2016/7/8.
 */
public class SqliteModelImpl implements SqliteModel {

    @Override
    public void insert(SQLiteDatabase db,String tableName,OnSqliteFinishListener onSqliteFinishListener) {
        ContentValues values=new ContentValues();
        //  开始组装第一条数据
        values.put("name", "The Da Vinci Code");
        values.put("author", "Dan Brown");
        values.put("pages", 454);
        values.put("price", 16.96);
        long result1 = db.insert(tableName, null, values);
        values.clear();
        //  开始组装第二条数据
        values.put("name", "The Lost Symbol");
        values.put("author", "Dan Brown");
        values.put("pages", 510);
        values.put("price", 19.95);
        long result2= db.insert(tableName, null, values);

        if(result1==-1||result2==-1){
            onSqliteFinishListener.onInsertError();
        }else{
            onSqliteFinishListener.onInsertSuccess();
        }
    }
}
