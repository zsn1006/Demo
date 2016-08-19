package com.polycom.polycom.contentProvider;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.support.annotation.Nullable;
import android.util.Log;

import com.polycom.polycom.sqlite.MyDataBaseHelper;

/**
 * Created by zsn on 2016/8/5.
 */
public class MyContentProvider extends ContentProvider {
    public static final int BOOK_DIR=0;
    public static final int BOOK_ITEM=1;
    public static final int CATEGORY_DIR=2;
    public static final int CATEGORY_ITEM=3;
    public static final String AUTHORITY="com.polycom.polycom.contentProvider.provider";
    private static UriMatcher uriMatcher;
    private MyDataBaseHelper dbHelper;
    static{
        uriMatcher=new UriMatcher(UriMatcher.NO_MATCH);
        uriMatcher.addURI(AUTHORITY,"book",BOOK_DIR);
        uriMatcher.addURI(AUTHORITY,"book/#",BOOK_ITEM);
        uriMatcher.addURI(AUTHORITY,"category",CATEGORY_DIR);
        uriMatcher.addURI(AUTHORITY,"category/#",CATEGORY_ITEM);
    }



    //完成对数据库的创建和升级等操作，返回 true 表示内容提供器初始化成功，返回 false 则表示失败
    //只有当存在ContentResolver 尝试访问我们程序中的数据时，内容提供器才会被初始化
    @Override
    public boolean onCreate() {
        dbHelper=new MyDataBaseHelper(getContext(),"BookStore.db",null,2);
        Log.i("zsn","onCreate");
        return true;
    }

    //使用 uri 参数来确定查询哪张表，projection 参数用于确
    //定查询哪些列，selection 和 selectionArgs 参数用于约束查询哪些行，sortOrder 参数用于
    //对结果进行排序，查询的结果存放在 Cursor 对象中返回
    @Nullable
    @Override
    public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs, String sortOrder) {
        SQLiteDatabase db = dbHelper.getReadableDatabase();
        Cursor cursor=null;
        switch (uriMatcher.match(uri)){
            case BOOK_DIR:
                cursor=db.query(MyDataBaseHelper.TABLE_NAME_1,projection,selection,selectionArgs,null,null,sortOrder);
                break;
            case BOOK_ITEM:
                String bookId = uri.getPathSegments().get(1);
                cursor=db.query(MyDataBaseHelper.TABLE_NAME_1,projection,"id=?",new String[]{bookId},null,null,sortOrder);
                break;
            case CATEGORY_DIR:
                cursor=db.query(MyDataBaseHelper.TABLE_NAME_2,projection,selection,selectionArgs,null,null,sortOrder);
                break;
            case CATEGORY_ITEM:
                String category_Id = uri.getPathSegments().get(1);
                cursor=db.query(MyDataBaseHelper.TABLE_NAME_2,projection,"id=?",new String[]{category_Id},null,null,sortOrder);
                break;
            default:
                break;
        }
        return cursor;
    }

    //根据传入的内容 URI 来返回相应的 MIME 类型
    @Nullable
    @Override
    public String getType(Uri uri) {
        switch (uriMatcher.match(uri)){
            case BOOK_DIR:
                return "vnd.android.cursor.dir/vnd.com.polycom.polycom.contentProvider.provider.book";
            case BOOK_ITEM:
                return "vnd.android.cursor.item/vnd.com.polycom.polycom.contentProvider.provider.book";
            case CATEGORY_DIR:
                return "vnd.android.cursor.dir/vnd.com.polycom.polycom.contentProvider.provider.category";
            case CATEGORY_ITEM:
                return "vnd.android.cursor.item/vnd.com.polycom.polycom.contentProvider.provider.category";
        }
        return null;
    }

    //使用 uri 参数来确定要添加到的表，待添加的数据
    //保存在 values 参数中。添加完成后，返回一个用于表示这条新记录的 URI
    @Nullable
    @Override
    public Uri insert(Uri uri, ContentValues values) {
        SQLiteDatabase db=dbHelper.getWritableDatabase();
        Uri uriReturn=null;
        switch (uriMatcher.match(uri)){
            case BOOK_DIR:
            case BOOK_ITEM:
                Log.i("zsn","values=="+values.get("name"));
                long newBookId=db.insert(MyDataBaseHelper.TABLE_NAME_1, null, values);
                uriReturn = Uri.parse("content://" + AUTHORITY + "/book/" + newBookId);
                Log.i("zsn","uriReturn=="+uriReturn);
                break;
            case CATEGORY_DIR:
            case CATEGORY_ITEM:
                long newCategoryId = db.insert(MyDataBaseHelper.TABLE_NAME_2, null, values);
                uriReturn = Uri.parse("content://" + AUTHORITY + "/category/" +
                        newCategoryId);
                break;
            default:
                break;
        }
        return uriReturn;
    }

    //使用 uri 参数来确定删除哪一张表中的数据，selection
     //和 selectionArgs 参数用于约束删除哪些行，被删除的行数将作为返回值返回
    @Override
    public int delete(Uri uri, String selection, String[] selectionArgs){
        // 删除数据
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        int deletedRows = 0;
        switch (uriMatcher.match(uri)) {
            case BOOK_DIR:
                deletedRows = db.delete(MyDataBaseHelper.TABLE_NAME_1, selection, selectionArgs);
                break;
            case BOOK_ITEM:
                String bookId = uri.getPathSegments().get(1);
                deletedRows = db.delete(MyDataBaseHelper.TABLE_NAME_1, "id = ?", new String[]{bookId});
                break;
            case CATEGORY_DIR:
                deletedRows = db.delete(MyDataBaseHelper.TABLE_NAME_2, selection, selectionArgs);
                break;
            case CATEGORY_ITEM:
                String categoryId = uri.getPathSegments().get(1);
                deletedRows = db.delete(MyDataBaseHelper.TABLE_NAME_2, "id = ?", new String[]
                        {categoryId});
                break;
            default:
                break;
        }
        return deletedRows;
    }

    //使用 uri 参数来确定更新哪一张表中的数据，新数据保存在 values 参数中，selection 和
    // selectionArgs 参数用于约束更新哪些行，受影响的行数将作为返回值返回
    @Override
    public int update(Uri uri, ContentValues values, String selection, String[] selectionArgs) {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        int updatedRows=0;
        switch (uriMatcher.match(uri)){
            case BOOK_DIR:
                updatedRows=db.update(MyDataBaseHelper.TABLE_NAME_1,values,selection,selectionArgs);
                break;
            case BOOK_ITEM:
                String bookId = uri.getPathSegments().get(1);
                updatedRows=db.update(MyDataBaseHelper.TABLE_NAME_1,values,"id=?",new String[]{bookId});
                break;
            case CATEGORY_DIR:
                updatedRows = db.update(MyDataBaseHelper.TABLE_NAME_2, values, selection,
                        selectionArgs);
                break;
            case CATEGORY_ITEM:
                break;
            default:
                break;
        }
        return updatedRows;
    }
}
