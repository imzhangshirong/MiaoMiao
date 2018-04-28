package com.zhangshirong.miaomiao.View.Login;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.zhangshirong.miaomiao.Base.View.NActivity;
import com.zhangshirong.miaomiao.View.Loading.ActivityLoading;
import com.zhangshirong.miaomiao.View.Register.ActivityRegister;
import com.zhangshirong.miaomiao.View.Main.ActivityMain;
import com.zhangshirong.miaomiao.R;

public class ActivityLogin extends NActivity {
    TextView viewGoRegister;
    Button viewLoginButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        viewGoRegister = findViewById(R.id.login_go_register);
        viewGoRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(ActivityRegister.class);
            }
        });
        viewLoginButton = findViewById(R.id.login_button_login);
        viewLoginButton.setOnClickListener(new View.OnClickListener() {
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
