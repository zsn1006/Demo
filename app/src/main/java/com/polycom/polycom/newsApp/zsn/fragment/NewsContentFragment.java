package com.polycom.polycom.newsApp.zsn.fragment;

import android.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.polycom.polycom.R;

/**
 * Created by zsn on 2016/6/24.
 */
public class NewsContentFragment extends Fragment{
    private View view;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view=inflater.inflate(R.layout.news_content_frag,container,false);
        return view;
    }
    public void refresh(String newsContent,String newsTitle){
        View visibilityLayout=view.findViewById(R.id.visibility_layout);
        visibilityLayout.setVisibility(View.VISIBLE);
        TextView newsContentText=(TextView)view.findViewById(R.id.news_content);
        TextView newsTitleText=(TextView)view.findViewById(R.id.news_title);
        newsContentText.setText(newsContent);// 刷新新闻的内容
        newsTitleText.setText(newsTitle);// 刷新新闻的标题
    }
}
