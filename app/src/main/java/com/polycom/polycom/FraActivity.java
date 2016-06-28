package com.polycom.polycom;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.Window;

/**
 * Created by Administrator on 2016/6/14.
 */
    public class FraActivity extends FragmentActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_fragment);
        //Button button=(Button)findViewById(R.id.button);
       /* button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AnotherRightFragment anotherRightFragment=new AnotherRightFragment();
                FragmentManager fragmentManager=getFragmentManager();
                FragmentTransaction transaction = fragmentManager.beginTransaction();
                transaction.replace(R.id.right_layout,anotherRightFragment);
                transaction.addToBackStack(null);
                transaction.commit();
            }
        });*/
    }
}
