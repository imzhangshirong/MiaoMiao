package com.zhangshirong.miaomiao.View.Main.Note;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.R;

/**
 * Created by kyarvis on 18-4-26.
 */

public class FragmentNotebook extends NFragment {
    private View fragmentView;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        fragmentView = inflater.inflate(R.layout.fragment_main_note_notebook, container, false);
        return fragmentView;
    }
}
