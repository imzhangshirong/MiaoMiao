package com.zhangshirong.miaomiao.DataBinding;

import android.databinding.Bindable;

import com.zhangshirong.miaomiao.Base.ViewModel.ViewModel;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.NoteProxy;

public class MainNoteTestReportListCellVM extends ViewModel {
    private TestResult data;
    private NoteProxy noteProxy;
    private NoteBook noteBook;
    public MainNoteTestReportListCellVM(TestResult data){
        noteProxy = (NoteProxy) MiaoMiao.getProxy(NoteProxy.class);
        noteBook = noteProxy.getNoteBookById(data.bookId);
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
    @Bindable
    public int getFillableBlockCount(){
        return data.allFillBlock;
    }
    @Bindable
    public int getNotePageCount(){
        return noteBook.pages.size();
    }
    @Bindable
    public int getScore(){
        return (int)((float)data.correctFillBlock/(float)data.allFillBlock*100);
    }
    @Bindable
    public int getTimeUsed(){
        long d = data.updateTime - data.createTime;
        int min = (int)(d/60);
        return min;
    }
    @Bindable
    public boolean getIsCompleted(){
        return (data.endTime>0);
    }
}
