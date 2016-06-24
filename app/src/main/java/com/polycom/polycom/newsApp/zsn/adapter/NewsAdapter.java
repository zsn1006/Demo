package com.polycom.polycom.newsApp.zsn.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.polycom.polycom.R;
import com.polycom.polycom.newsApp.zsn.bean.News;

import java.util.List;

/**
 * Created by Administrator on 2016/6/24.
 */
public class NewsAdapter extends ArrayAdapter<News> {
    private int resourceId;
    private Context mContext;
    public NewsAdapter(Context context, int textViewResourceId, List<News> objects) {
        super(context, textViewResourceId, objects);
        this.resourceId=textViewResourceId;
        this.mContext=context;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
       News news=getItem(position);
        View view;
        if(convertView==null){
            view=LayoutInflater.from(mContext).inflate(resourceId,null);
        }else{
            view=convertView;
        }
        TextView newsTextView=(TextView)view.findViewById(R.id.news_title);
        newsTextView.setText(news.getTitle());
        return view;
    }
}
