package com.polycom.polycom;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.List;

/**
 * Created by Administrator on 2016/6/7.
 */
public class MsgAdapter extends ArrayAdapter<Msg> {

    private int resourceId;
    public MsgAdapter(Context context, int textViewResourceId, List<Msg> objects) {
        super(context, textViewResourceId, objects);
        this.resourceId=textViewResourceId;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        Msg msg = getItem(position);
        View view;
        ViewHolder viewHolder;
        if(convertView==null){
            view = LayoutInflater.from(getContext()).inflate(resourceId, null);
            viewHolder=new ViewHolder();
            viewHolder.leftLinearLayout=(LinearLayout)view.findViewById(R.id.left_layout);
            viewHolder.rightLinearLayout=(LinearLayout)view.findViewById(R.id.right_layout);
            viewHolder.leftMsg=(TextView)view.findViewById(R.id.left_msg);
            viewHolder.rightMsg=(TextView)view.findViewById(R.id.right_msg);
            view.setTag(viewHolder);
        }else{
            view=convertView;
            viewHolder=(ViewHolder)convertView.getTag();
        }

        if(msg.getType()==Msg.TYPE_RECEIVED){
            viewHolder.leftLinearLayout.setVisibility(View.VISIBLE);
            viewHolder.rightLinearLayout.setVisibility(View.GONE);
            viewHolder.leftMsg.setText(msg.getContent());
        }else if(msg.getType()==Msg.TYPE_SEND){
            viewHolder.leftLinearLayout.setVisibility(View.GONE);
            viewHolder.rightLinearLayout.setVisibility(View.VISIBLE);
            viewHolder.rightMsg.setText(msg.getContent());
        }
        return view;
    }

    private class ViewHolder{
        LinearLayout leftLinearLayout;
        LinearLayout rightLinearLayout;
        TextView leftMsg;
        TextView rightMsg;
    }
}
