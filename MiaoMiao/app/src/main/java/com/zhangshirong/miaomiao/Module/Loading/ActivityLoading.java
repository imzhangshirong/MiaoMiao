package com.zhangshirong.miaomiao.Module.Loading;

import android.os.Bundle;

import com.zhangshirong.miaomiao.ActivityStart;
import com.zhangshirong.miaomiao.Module.Main.ActivityMain;
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
                            startActivity(ActivityMain.class);
                        }
                    });
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }
}
