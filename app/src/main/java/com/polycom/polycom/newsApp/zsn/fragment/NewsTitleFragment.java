package com.polycom.polycom.newsApp.zsn.fragment;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import com.polycom.polycom.R;
import com.polycom.polycom.newsApp.zsn.activity.NewsContentActivity;
import com.polycom.polycom.newsApp.zsn.adapter.NewsAdapter;
import com.polycom.polycom.newsApp.zsn.bean.News;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Administrator on 2016/6/24.
 */
public class NewsTitleFragment extends Fragment implements AdapterView.OnItemClickListener {
    private List<News> newsList;
    private NewsAdapter adapter;
    private ListView newsTitleListView;
    private boolean isTwoPane;

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        newsList=getNews();
        adapter=new NewsAdapter(context, R.layout.news_item,newsList);
    }

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.news_title_frag,container,false);
        newsTitleListView = (ListView) view.findViewById(R.id.news_title_list_view);
        newsTitleListView.setAdapter(adapter);
        newsTitleListView.setOnItemClickListener(this);
        return view;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        if(getActivity().findViewById(R.id.news_content_layout)!=null){
            isTwoPane=true;// 可以找到news_content_layout布局时，为双页模式
        }else{
            isTwoPane=false;// 不可以找到news_content_layout布局时，为单页模式
        }
    }

    private List<News> getNews() {
        List<News> newsList=new ArrayList<>();
        News news=new News();
        news.setTitle("Succeed in College as a Learning Disabled Student");
        news.setContent("College freshmen will soon learn to live with a " +
                "roommate, adjust to a new social scene and survive " +
                "less-than-stellar dining hall food. Students with learning " +
                "disabilities will face these transitions while also" +
                " grappling with a few more hurdles.");
        newsList.add(news);
        News news1 = new News();
        news1.setTitle("Google Android exec poached by China's Xiaomi");
        news1.setContent("China's Xiaomi has poached a key Google executive" +
                " involved in the tech giant's Android phones, in a move " +
                "seen as a coup for the rapidly growing Chinese smartphone " +
                "maker.");
        newsList.add(news1);
        return newsList;
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        News news=newsList.get(position);
        if(isTwoPane){
            // 如果是双页模式，则刷新NewsContentFragment中的内容
            NewsContentFragment newsContentFragment = (NewsContentFragment)
            getFragmentManager().findFragmentById(R.id.news_content_fragment);
            newsContentFragment.refresh(news.getContent(),news.getTitle());
        }else{
            // 如果是单页模式，则直接启动NewsContentActivity
            NewsContentActivity.actionStart(getActivity(),news.getTitle(),news.getContent());
        }
    }
}
