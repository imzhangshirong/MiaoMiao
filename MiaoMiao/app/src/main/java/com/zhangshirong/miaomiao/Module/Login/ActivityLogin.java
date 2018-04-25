package com.zhangshirong.miaomiao.Module.Login;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.zhangshirong.miaomiao.Base.View.NActivity;
import com.zhangshirong.miaomiao.Module.Loading.ActivityLoading;
import com.zhangshirong.miaomiao.Module.Register.ActivityRegister;
import com.zhangshirong.miaomiao.R;

public class ActivityLogin extends NActivity {
    TextView view_goRegister;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        view_goRegister = (TextView)findViewById(R.id.login_go_register);
        view_goRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(ActivityRegister.class);
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        endActivity(ActivityLoading.class);
    }
}
