package com.polycom.polycom.contentProvider;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.polycom.polycom.R;

import java.util.ArrayList;

/**
 * Created by zsn on 2016/7/29.
 */
public class RecyclerAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    //有header
    public static final int TYPE_HEADER=0;
    //有footer
    public static final int TYPE_FOOTER=1;
    //没有header,footer
    public static final int TYPE_NORMAL=2;
    private ArrayList<String>mList;
    private View mHeaderView;
    private View mFooterView;
    public RecyclerAdapter(Context context,ArrayList<String> list) {
        this.mList=list;
    }

    public View getmFooterView() {
        return mFooterView;
    }

    public void setmFooterView(View mFooterView) {
        this.mFooterView = mFooterView;
        notifyItemInserted(getItemCount()-1);
    }

    public View getmHeaderView() {
        return mHeaderView;
    }

    public void setmHeaderView(View mHeaderView) {
        this.mHeaderView = mHeaderView;
        notifyItemInserted(0);
    }

    @Override
    public int getItemViewType(int position) {
        if(mHeaderView==null&&mFooterView==null){
            return TYPE_NORMAL;
        }
        if(position==0){
            return TYPE_HEADER;
        }
        if(position==getItemCount()-1){
            return TYPE_FOOTER;
        }
        return TYPE_NORMAL;
    }
    //创建View，如果是HeaderView或者是FooterView，直接在Holder中返回
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        if(mHeaderView!=null&&viewType==TYPE_HEADER){
            return new ListHolder(mHeaderView);
        }
        if(mFooterView!=null&&viewType==TYPE_FOOTER){
            return new ListHolder(mFooterView);
        }
        View layout = LayoutInflater.from(parent.getContext()).inflate(R.layout.recycler_view_item, parent,false);
        return new ListHolder(layout);
    }
    //绑定View,这里是根据返回的这个position的类型,从而进行绑定的,HeaderView和FooterView,就不同绑定了
    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        if(getItemViewType(position)==TYPE_NORMAL){
            if(holder instanceof ListHolder){
                ((ListHolder)holder).mrecyclerViewItem.setText(mList.get(position-1));
            }
            return;
        }else if(getItemViewType(position) == TYPE_HEADER){
            return;
        }else{
            return;
        }
    }

    class ListHolder extends RecyclerView.ViewHolder{

        private  TextView mrecyclerViewItem;
        public ListHolder(View itemView) {
            super(itemView);
            if(itemView==mHeaderView){
                return;
            }
            if(itemView==mFooterView){
                return;
            }
            mrecyclerViewItem=(TextView)itemView.findViewById(R.id.item_recycler_view);
        }
    }
    @Override
    public int getItemCount() {
        if(mHeaderView==null&&mFooterView==null){
           return mList.size();
        }
        else if(mHeaderView==null&&mFooterView!=null){
            return mList.size()+1;
        }
        else if(mHeaderView!=null&&mFooterView==null){
            return mList.size()+1;
        }else {
            return mList.size()+2;
        }
    }
}
