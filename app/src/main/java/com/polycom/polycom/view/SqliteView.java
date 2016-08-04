package com.polycom.polycom.view;

/**
 * Created by zsn on 2016/6/13.
 */
public interface SqliteView {
    void setInsertSuccess();
    void setInsertError();
    void setUpdateSuccess();
    void setUpdateError();
    void setDeleteSuccess();
    void setDeleteError();
    void setReplaceDataSuccess();
    void setReplaceDataError();
}
