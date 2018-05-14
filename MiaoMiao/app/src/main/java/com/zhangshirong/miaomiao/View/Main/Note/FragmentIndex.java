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
import com.zhangshirong.miaomiao.DataBinding.MainNoteIndexBlockTestListCellVM;
import com.zhangshirong.miaomiao.DataBinding.MainNoteIndexVM;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.TestProxy;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.ListAdapter;

import java.util.ArrayList;

public class FragmentIndex extends NFragment {
    private View fragmentView;
    private ViewDataBinding binding;
    private MainNoteIndexVM viewModel;
    private TestProxy testProxy;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        //fragmentView = inflater.inflate(R.layout.fragment_main_note_index, container, false);
        binding = DataBindingUtil.inflate(inflater,R.layout.fragment_main_note_index, container, false);
        fragmentView = binding.getRoot();
        init();
        return fragmentView;
    }
    void init(){
        testProxy = (TestProxy) MiaoMiao.getProxy(TestProxy.class);
        setDataBinding();
    }
    void setDataBinding(){
        viewModel = new MainNoteIndexVM();
        binding.setVariable(BR.mainNoteIndex,viewModel);
        ArrayList<TestResult> list = testProxy.getAllTestResult();
        ListAdapter<MainNoteIndexBlockTestListCellVM,TestResult> listAdapter = new ListAdapter<>(
                getContext(),
                MainNoteIndexBlockTestListCellVM.class,
                list,
                R.layout.cell_main_note_block_testcount,
                BR.mainNoteBlockTestCountListCell);
        binding.setVariable(BR.mainNoteBlockTestCountListAdapter,listAdapter);
    }
}

