package com.zhangshirong.miaomiao.Module.Main;

import android.databinding.DataBindingUtil;
import android.os.Bundle;

import com.zhangshirong.miaomiao.Base.View.NFragmentActivity;
import com.zhangshirong.miaomiao.DataBinding.UserDataVM;
import com.zhangshirong.miaomiao.Module.Loading.ActivityLoading;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.databinding.ActivityMainBinding;

public class ActivityMain extends NFragmentActivity {
    private ActivityMainBinding binding;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        UserDataVM userData = new UserDataVM();
        binding.setUserData(userData);
        binding.notifyChange();
    }

    @Override
    protected void onStart() {
        super.onStart();
        endActivity(ActivityLoading.class);
    }
}
