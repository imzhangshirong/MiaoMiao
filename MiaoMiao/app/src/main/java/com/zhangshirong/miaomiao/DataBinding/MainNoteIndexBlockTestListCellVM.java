package com.zhangshirong.miaomiao.DataBinding;

import android.databinding.Bindable;

import com.zhangshirong.miaomiao.Base.ViewModel.ViewModel;
import com.zhangshirong.miaomiao.Common;
import com.zhangshirong.miaomiao.DataObject.NoteData;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.NoteProxy;

import java.util.ArrayList;
import java.util.Calendar;

public class MainNoteIndexBlockTestListCellVM extends ViewModel {
    private TestResult data;
    public MainNoteIndexBlockTestListCellVM(TestResult data){
        this.data = data;
    }
    @Bindable
    public String getName(){

        return data.name;
    }
    @Bindable
    public int getPercent(){
        return (int)(((float)data.curPosition/(float)data.endPosition)*100);
    }
}
