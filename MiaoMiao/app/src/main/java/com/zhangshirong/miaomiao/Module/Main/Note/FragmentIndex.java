package com.zhangshirong.miaomiao.Module.Main.Note;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.R;

public class FragmentIndex extends NFragment {
    private View fragmentView;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        fragmentView = inflater.inflate(R.layout.fragment_main_note_index, container, false);
        return fragmentView;
    }
}

