package com.zhangshirong.miaomiao.View.Loading;

import android.os.Bundle;
import android.widget.Toast;

import com.zhangshirong.miaomiao.ActivityStart;
import com.zhangshirong.miaomiao.CPPInterface;
import com.zhangshirong.miaomiao.View.Login.ActivityLogin;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Base.View.NActivity;

public class ActivityLoading extends NActivity {



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loading);
    }

    @Override
    protected void onStart() {
        super.onStart();
        endActivity(ActivityStart.class);
        Toast.makeText(context, CPPInterface.stringFromJNI(),Toast.LENGTH_LONG).show();
        loadDelay();
    }

    void loadDelay(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(3000);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            startActivity(ActivityLogin.class);
                        }
                    });
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }
}
