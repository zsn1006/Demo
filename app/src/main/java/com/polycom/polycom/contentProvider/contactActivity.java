package com.polycom.polycom.contentProvider;

import android.database.Cursor;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;

import com.polycom.polycom.BaseActivity;
import com.polycom.polycom.R;
import com.socks.library.KLog;

import java.util.ArrayList;

/**
 * Created by zsn on 2016/7/29.
 */
public class ContactActivity extends BaseActivity{
    private RecyclerView mRecyclerView;
    private ArrayList<String> mList;
    private RecyclerAdapter mRecyclerAdapter;
    private ArrayList<String> contactsList;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_contact);
        KLog.i("zsn", "信息===============");
        KLog.d("zsn", "debug模式");
        KLog.a("zsn", "呵呵呵");
        KLog.e("zsn", "错误");
        KLog.v("zsn", "哎哟………");
        KLog.w("zsn","警告----------------------");
        initView();
        readContacts();
        //initData();
        mRecyclerAdapter=new RecyclerAdapter(this,contactsList);
        mRecyclerView.setAdapter(mRecyclerAdapter);

        //为RecyclerView添加HeaderView和FooterView
        setHeaderView(mRecyclerView);
        setFooterView(mRecyclerView);
        //分界线
        mRecyclerView.addItemDecoration(new MyDecoration(this,MyDecoration.VERTICAL_LIST));
    }

    private void setHeaderView(RecyclerView view){
        View header = LayoutInflater.from(this).inflate(R.layout.header, view, false);
        mRecyclerAdapter.setmHeaderView(header);
    }

    private void setFooterView(RecyclerView view){
        View footer = LayoutInflater.from(this).inflate(R.layout.footer, view, false);
        mRecyclerAdapter.setmFooterView(footer);
    }
    private void initData() {
        mList = new ArrayList<String>();
        for (int i = 0; i < 20; i++){
            mList.add("item" + i);
        }
    }
    private void readContacts() {
        contactsList = new ArrayList<String>();
        Cursor cursor = null;
        try {
            // 查询联系人数据
            cursor = getContentResolver().query(
                    ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
                    null, null, null, null);
            while (cursor.moveToNext()) {
            // 获取联系人姓名
                String displayName = cursor.getString(cursor.getColumnIndex(
                        ContactsContract.CommonDataKinds.Phone.DISPLAY_NAME));
            // 获取联系人手机号
                String number = cursor.getString(cursor.getColumnIndex(
                        ContactsContract.CommonDataKinds.Phone.NUMBER));
                contactsList.add(displayName + "\r" + number);
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (cursor != null) {
                cursor.close();
            }
        }
    }
    private void initView() {
        mRecyclerView = (RecyclerView)findViewById(R.id.contact_recycler_view);
        LinearLayoutManager linearLayoutManager=new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(linearLayoutManager);
    }
}
