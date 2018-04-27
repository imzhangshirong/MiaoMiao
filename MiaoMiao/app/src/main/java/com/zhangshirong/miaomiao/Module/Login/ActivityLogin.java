package com.zhangshirong.miaomiao.Module.Login;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.zhangshirong.miaomiao.Base.View.NActivity;
import com.zhangshirong.miaomiao.Module.Loading.ActivityLoading;
import com.zhangshirong.miaomiao.Module.Register.ActivityRegister;
import com.zhangshirong.miaomiao.Module.Main.ActivityMain;
import com.zhangshirong.miaomiao.R;

public class ActivityLogin extends NActivity {
    TextView view_goRegister;
    Button view_loginButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        view_goRegister = findViewById(R.id.login_go_register);
        view_goRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(ActivityRegister.class);
            }
        });
        view_loginButton = findViewById(R.id.login_button_login);
        view_loginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(ActivityMain.class);
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        endActivity(ActivityLoading.class);
    }
}
