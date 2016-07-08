package com.polycom.polycom;

import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends BaseActivity {

    private ListView msgListView;
    private EditText inputText;
    private Button send,close,save;
    private MsgAdapter adapter;
    private List<Msg>msgList=new ArrayList<>();
    private String content;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        initMsgs();
        initListener();
        adapter=new MsgAdapter(MainActivity.this,R.layout.msg_item,msgList);
        msgListView.setAdapter(adapter);
    }

    private void initListener() {
        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String content = inputText.getText().toString();
                if (!"".equals(content)) {
                    Msg msg = new Msg(content, Msg.TYPE_SEND);
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
        close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent=new Intent("com.polycom.polycom.FORCE_OFFLINE");
                sendBroadcast(intent);
            }
        });
        save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                save(inputText.getText().toString());
                startAty(MySqliteActivity.class);
            }
        });
    }

    /**
     * 保存数据到文件中
     */
    private void save(String content) {
        FileOutputStream out=null;
        BufferedWriter bufferedWriter = null;
        try {
            out = openFileOutput("data", MODE_APPEND);
            bufferedWriter=new BufferedWriter(new OutputStreamWriter(out));
            bufferedWriter.write(content);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            if(bufferedWriter!=null){
                try {
                    bufferedWriter.close();
                    bufferedWriter=null;
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        }

    }

    /**
     * 读取文件中的数据
     * @return
     */
    private String load(){
        FileInputStream in=null;
        BufferedReader reader=null;
        StringBuilder builder=new StringBuilder();
        try {
            in = openFileInput("data");
            reader=new BufferedReader(new InputStreamReader(in));
            String line="";
            while((line=reader.readLine())!=null){
                builder.append(line);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            if(reader!=null){
                try {
                    reader.close();
                    reader=null;
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        }
        return builder.toString();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        save(inputText.getText().toString());
    }

    private void initView() {
        inputText=(EditText)findViewById(R.id.input_text);
        send=(Button)findViewById(R.id.send);
        close=(Button)findViewById(R.id.close_all_aty);
        save=(Button)findViewById(R.id.save_content);
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

        content=load();
        if(!TextUtils.isEmpty(content)){
            inputText.setText(content);
            inputText.setSelection(content.length());
        }
    }
}
