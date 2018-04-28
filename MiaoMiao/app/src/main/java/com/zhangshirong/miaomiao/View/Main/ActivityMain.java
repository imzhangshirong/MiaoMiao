package com.zhangshirong.miaomiao.View.Main;

import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;

import com.zhangshirong.miaomiao.Base.View.NFragmentActivity;
import com.zhangshirong.miaomiao.DataBinding.UserDataVM;
import com.zhangshirong.miaomiao.View.Loading.ActivityLoading;
import com.zhangshirong.miaomiao.View.Main.Me.FragmentMeMain;
import com.zhangshirong.miaomiao.View.Main.Note.FragmentNoteMain;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.View.TakeNewNote.ActivityTakeNewNote;
import com.zhangshirong.miaomiao.databinding.ActivityMainBinding;

public class ActivityMain extends NFragmentActivity implements View.OnClickListener {
    private ActivityMainBinding binding;

    private FragmentNoteMain fragmentNote;
    private FragmentMeMain fragmentMe;

    private RelativeLayout viewTakeNewNote;

    private int position = 0;


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        UserDataVM userData = new UserDataVM();
        binding.setUserData(userData);
        binding.notifyChange();

        initView();
    }

    @Override
    protected void onStart() {
        super.onStart();
        endActivity(ActivityLoading.class);


    }

    private void initView(){
        viewTakeNewNote = findViewById(R.id.main_button_takenewnote);
        viewTakeNewNote.setOnClickListener(this);

        initFragment();
    }

    private void initFragment(){
        FragmentManager fm= getSupportFragmentManager();
        FragmentTransaction transaction =fm.beginTransaction();
        if(fragmentNote!=null){
            transaction.remove(fragmentNote);
        }
        if(fragmentMe!=null){
            transaction.remove(fragmentMe);
        }
        transaction.commit();
        fragmentNote=null;
        fragmentMe=null;
        setNav(position);
    }
    private void setNav(int id){
        position=id;
        FragmentManager fm= getSupportFragmentManager();
        FragmentTransaction transaction =fm.beginTransaction();
        hideFragment(transaction);
        resetNav();
        switch (id){
            case 0:
                if(fragmentNote==null){
                    fragmentNote=new FragmentNoteMain();
                    transaction.add(R.id.main_fragment,fragmentNote);
                }
                transaction.show(fragmentNote);
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
        }
        transaction.commit();
    }
    private void resetNav(){


    }
    private void hideFragment(android.support.v4.app.FragmentTransaction transaction){
        if(fragmentNote!=null){
            transaction.hide(fragmentNote);
        }
        if(fragmentMe!=null){
            transaction.hide(fragmentMe);
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.main_button_takenewnote:
                startActivity(ActivityTakeNewNote.class);
                break;
        }
    }
}
