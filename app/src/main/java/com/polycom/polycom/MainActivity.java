package com.polycom.polycom;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private ListView msgListView;
    private EditText inputText;
    private Button send;
    private MsgAdapter adapter;
    private List<Msg>msgList=new ArrayList<>();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //test github ---zsn
        initMsgs();
        initView();
        initListener();
        adapter=new MsgAdapter(MainActivity.this,R.layout.msg_item,msgList);
        msgListView.setAdapter(adapter);
    }

    private void initListener() {
        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String content=inputText.getText().toString();
                if(!"".equals(content)){
                    Msg msg=new Msg(content,Msg.TYPE_SEND);
                    msgList.add(msg);
                    //刷新最新的数据
                    adapter.notifyDataSetChanged();
                    //定位到最后一行
                    msgListView.setSelection(msgList.size());
                    //清空编辑框
                    inputText.setText("");
                }
            }
        });
    }

    private void initView() {
        inputText=(EditText)findViewById(R.id.input_text);
        send=(Button)findViewById(R.id.send);
        msgListView=(ListView)findViewById(R.id.msg_list_view);
    }

    /**
     * 初始化消息数据
     */
    private void initMsgs() {
        Msg msg1=new Msg("你好",Msg.TYPE_RECEIVED);
        msgList.add(msg1);
        Msg msg2=new Msg("嗯，你好",Msg.TYPE_SEND);
        msgList.add(msg2);
        Msg msg3=new Msg("都好",Msg.TYPE_RECEIVED);
        msgList.add(msg3);
    }
}
