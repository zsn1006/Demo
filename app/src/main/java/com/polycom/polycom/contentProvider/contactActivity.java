package com.polycom.polycom.contentProvider;

import android.database.Cursor;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.support.v4.widget.SwipeRefreshLayout;
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
public class contactActivity extends BaseActivity{
    private RecyclerView mRecyclerView;
    private ArrayList<String> mList;
    private RecyclerAdapter mRecyclerAdapter;
    private ArrayList<String> contactsList;
    private SwipeRefreshLayout mSwipeRefreshLayout;
    private LinearLayoutManager mLinearLayoutManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_contact);
        KLog.i("zsn", "信息===============");
        KLog.d("zsn", "debug模式");
        KLog.a("zsn", "呵呵呵");
        KLog.e("zsn", "错误");
        KLog.v("zsn", "哎哟………");
        KLog.w("zsn", "警告----------------------");
        initView();
        initListener();
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

    private void initListener() {
        mSwipeRefreshLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                contactsList.add(0, "嘿，我是“下拉刷新”生出来的");
                //数据重新加载完成后，提示数据发生改变，并且设置现在不在刷新
                mRecyclerAdapter.notifyDataSetChanged();
                mSwipeRefreshLayout.setRefreshing(false);
            }
        });
        mRecyclerView.setOnScrollListener(new EndLessOnScrollListener(mLinearLayoutManager) {
            @Override
            public void onLoadMore(int currentPage) {
                loadMoreData(currentPage);
            }
        });
    }

    private void setHeaderView(RecyclerView view){
        View header = LayoutInflater.from(this).inflate(R.layout.header, view, false);
        mRecyclerAdapter.setmHeaderView(header);
    }

    private void setFooterView(RecyclerView view){
        View footer = LayoutInflater.from(this).inflate(R.layout.footer, view, false);
        mRecyclerAdapter.setmFooterView(footer);
    }
    /*private void initData() {
        mList = new ArrayList<String>();
        for (int i = 0; i < 20; i++){
            mList.add("item" + i);
        }
    }*/
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
    private void loadMoreData(int currentPage) {
        KLog.w("zsn", "currentPage: " + currentPage);
        for (int i =0; i < 10; i++){
            contactsList.add("嘿，我是“上拉加载”生出来的"+i);
            mRecyclerAdapter.notifyDataSetChanged();
        }
    }
    private void initView() {
        mRecyclerView = (RecyclerView)findViewById(R.id.contact_recycler_view);
        mLinearLayoutManager=new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLinearLayoutManager);
        mSwipeRefreshLayout=(SwipeRefreshLayout)findViewById(R.id.layout_swipe_refresh);
    }
}
