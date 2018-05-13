package com.zhangshirong.miaomiao.View.Main.Me;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.R;

public class FragmentMeMain extends NFragment{
    private View fragmentView;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        fragmentView = inflater.inflate(R.layout.fragment_main_me, container, false);
        return fragmentView;
    }
}
