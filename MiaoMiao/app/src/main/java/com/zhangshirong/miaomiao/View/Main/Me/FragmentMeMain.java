package com.zhangshirong.miaomiao.View.Main.Me;

import android.databinding.DataBindingUtil;
import android.databinding.ViewDataBinding;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.BR;
import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.DataBinding.MainMeVM;
import com.zhangshirong.miaomiao.DataBinding.MainNoteIndexBlockTestListCellVM;
import com.zhangshirong.miaomiao.DataBinding.MainNoteIndexVM;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.TestProxy;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.ListAdapter;

import java.util.ArrayList;

public class FragmentMeMain extends NFragment{
    private View fragmentView;
    private ViewDataBinding binding;
    private MainMeVM viewModel;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        //fragmentView = inflater.inflate(R.layout.fragment_main_note_index, container, false);
        binding = DataBindingUtil.inflate(inflater,R.layout.fragment_main_me, container, false);
        fragmentView = binding.getRoot();
        init();
        return fragmentView;
    }
    void init(){
        setDataBinding();
    }
    void setDataBinding(){
        viewModel = new MainMeVM();
        binding.setVariable(BR.mainMe,viewModel);
    }
}
