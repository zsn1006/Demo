package com.polycom.polycom.sqlite;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import com.polycom.polycom.utils.ToastUtil;

/**
 * Created by zsn on 2016/7/8.
 */
public class MyDataBaseHelper extends SQLiteOpenHelper {
    public static  String TABLE_NAME_1="book";
    public static  String TABLE_NAME_2="category";
    private static  int DB_VERSION=1;
    private Context mContext;
    private static final String CREATE_BOOK="create table "+TABLE_NAME_1+"(" +
            "_id integer primary key autoincrement,author text,price real," +
            "pages integer,name text)";
    private static final String CREATE_CATEGORY="create table "+TABLE_NAME_2+"(" +
            "_id integer primary key autoincrement,category_name text,category_code integer)";

    public MyDataBaseHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        super(context, name, factory, version);
        this.mContext=context;
    }


    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(CREATE_BOOK);
        db.execSQL(CREATE_CATEGORY);
        ToastUtil.showToast(mContext,"Create succeeded");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("drop table if exists book");
        db.execSQL("drop table if exists category");
        onCreate(db);
    }
}
