package com.zhangshirong.miaomiao;

import android.os.Bundle;

import com.zhangshirong.miaomiao.Base.View.NActivity;
import com.zhangshirong.miaomiao.Module.Loading.ActivityLoading;

public class ActivityStart extends NActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start);
    }

    @Override
    protected void onStart() {
        super.onStart();
        startActivity(ActivityLoading.class);
    }
}
