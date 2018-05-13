package com.zhangshirong.miaomiao.View.Main.Note;

import android.databinding.Bindable;
import android.databinding.DataBindingUtil;
import android.databinding.ViewDataBinding;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.BR;
import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.DataBinding.MainNoteIndexVM;
import com.zhangshirong.miaomiao.R;

public class FragmentIndex extends NFragment {
    private View fragmentView;
    private ViewDataBinding binding;
    private MainNoteIndexVM viewModel;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        //fragmentView = inflater.inflate(R.layout.fragment_main_note_index, container, false);
        binding = DataBindingUtil.inflate(inflater,R.layout.fragment_main_note_index, container, false);
        viewModel = new MainNoteIndexVM();
        fragmentView = binding.getRoot();
        binding.setVariable(BR.MainNoteIndexVM,viewModel);
        return fragmentView;
    }
}

