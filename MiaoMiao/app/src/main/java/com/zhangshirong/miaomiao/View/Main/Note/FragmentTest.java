package com.zhangshirong.miaomiao.View.Main.Note;

import android.databinding.DataBindingUtil;
import android.databinding.ViewDataBinding;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.BR;
import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.DataBinding.MainNoteNotebookGridCellVM;
import com.zhangshirong.miaomiao.DataBinding.MainNoteTestReportListCellVM;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.NoteProxy;
import com.zhangshirong.miaomiao.Proxy.TestProxy;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.ListAdapter;

import java.util.ArrayList;

/**
 * Created by kyarvis on 18-4-26.
 */

public class FragmentTest extends NFragment {
    private View fragmentView;
    private ViewDataBinding binding;
    private TestProxy testProxy;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        binding = DataBindingUtil.inflate(inflater,R.layout.fragment_main_note_test,container, false);;
        fragmentView = binding.getRoot();
        init();
        return fragmentView;
    }
    void init(){
        testProxy = (TestProxy) MiaoMiao.getProxy(TestProxy.class);
        setDataBinding();
    }
    void setDataBinding(){
        ArrayList<TestResult> list = testProxy.getAllTestResult();
        ListAdapter<MainNoteTestReportListCellVM,TestResult> listAdapter = new ListAdapter<>(
                getContext(),
                MainNoteTestReportListCellVM.class,
                list,
                R.layout.cell_main_test_report,
                BR.mainNoteTestReportListCell);
        binding.setVariable(BR.mainNoteTestReportListAdapter,listAdapter);
    }
}
