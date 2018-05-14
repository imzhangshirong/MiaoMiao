package com.zhangshirong.miaomiao.DataBinding;

import android.databinding.Bindable;

import com.zhangshirong.miaomiao.Base.ViewModel.ViewModel;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;

public class MainNoteNotebookGridCellVM extends ViewModel {
    private NoteBook data;
    public MainNoteNotebookGridCellVM(NoteBook data){
        this.data = data;
    }
    @Bindable
    public String getName(){
        return data.name;
    }

    @Bindable
    public String getCover(){
        return data.coverImg;
    }
}
