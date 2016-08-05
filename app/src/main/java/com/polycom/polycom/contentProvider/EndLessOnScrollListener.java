package com.polycom.polycom.contentProvider;

import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import com.socks.library.KLog;

/**
 * Created by zsn on 2016/8/5.
 */
public abstract class EndLessOnScrollListener extends RecyclerView.OnScrollListener {
    //声明一个LinearLayoutManager
    private LinearLayoutManager mLinearLayoutManager;
    //在屏幕上可见的item数量
    private int visibleItemCount;
    //已经加载出来的Item的数量
    private int totalItemCount;
    //主要用来存储上一个totalItemCount
    private int previousTotal = 0;
    //在屏幕可见的Item中的第一个
    private int firstVisibleItem;
    //是否正在上拉数据
    private boolean isLoading = true;
    //当前页，从0开始
    private int currentPage = 0;

    public EndLessOnScrollListener(LinearLayoutManager linearLayoutManager) {
        this.mLinearLayoutManager = linearLayoutManager;
    }

    @Override
    public void onScrolled(RecyclerView recyclerView, int dx, int dy) {
        super.onScrolled(recyclerView, dx, dy);
        visibleItemCount = recyclerView.getChildCount();
        totalItemCount = mLinearLayoutManager.getItemCount();
        firstVisibleItem = mLinearLayoutManager.findFirstVisibleItemPosition();
        if (isLoading) {
            KLog.i("zsn", "firstVisibleItem: " + firstVisibleItem);
            KLog.i("zsn", "totalPageCount: " + totalItemCount);
            KLog.i("zsn", "visibleItemCount: " + visibleItemCount);
            KLog.i("zsn", "previousTotal: " + previousTotal);
            if (totalItemCount > previousTotal) {
                //说明数据已经加载结束
                isLoading = false;
                previousTotal = totalItemCount;
            }

        }
        if (!isLoading && totalItemCount - visibleItemCount <= firstVisibleItem) {
            currentPage++;
            onLoadMore(currentPage);
            isLoading = true;
        }
        KLog.i("zsn", "currentPage: " + currentPage);
    }

    /**
     * 提供一个抽闲方法，在Activity中监听到这个EndLessOnScrollListener
     * 并且实现这个方法
     */
    public abstract void onLoadMore(int currentPage);
}
