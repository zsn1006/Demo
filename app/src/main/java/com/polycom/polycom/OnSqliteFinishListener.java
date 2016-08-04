package com.polycom.polycom;

/**
 *登陆事件的监听
 * Created by zsn on 2016/7/8.
 */
public interface OnSqliteFinishListener {
    void onInsertSuccess();
    void onInsertError();
    void onUpdateSuccess();
    void onUpdateError();
    void onDeleteSuccess();
    void onDeleteError();
    void onReplaceDataSuccess();
    void onReplaceDataError();
}
