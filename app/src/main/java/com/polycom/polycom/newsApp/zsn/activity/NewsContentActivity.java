package com.polycom.polycom.newsApp.zsn.activity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Window;

import com.polycom.polycom.R;
import com.polycom.polycom.newsApp.zsn.fragment.NewsContentFragment;

/**
 * Created by Administrator on 2016/6/24.
 */
public class NewsContentActivity extends FragmentActivity {

    public static void actionStart(Context context,String newsTitle,String  newsContent){
        Intent intent=new Intent(context,NewsContentActivity.class);
        intent.putExtra("news_title",newsTitle);
        intent.putExtra("news_content",newsContent);
        context.startActivity(intent);
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.news_content);
        // 获取传入的新闻标题
        String newsTitle = getIntent().getStringExtra("news_title");
        // 获取传入的新闻内容
        String newsContent = getIntent().getStringExtra("news_content");

        NewsContentFragment newsContentFragment = (NewsContentFragment)getSupportFragmentManager().findFragmentById(R.id.news_content_fragment);
        //刷新新闻fragment界面
        newsContentFragment.refresh(newsContent,newsTitle);


    }
}
