package com.zhangshirong.miaomiao.DataBinding;

import android.databinding.Bindable;

import com.zhangshirong.miaomiao.Base.ViewModel.ViewModel;
import com.zhangshirong.miaomiao.Common;
import com.zhangshirong.miaomiao.DataObject.NoteData;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.NoteProxy;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class MainNoteIndexVM extends ViewModel {
    private NoteProxy noteProxy;
    public MainNoteIndexVM(){
        noteProxy = (NoteProxy) MiaoMiao.getProxy(NoteProxy.class);
    }
    @Bindable
    public int getDay(){
        Calendar cd = Calendar.getInstance();
        return cd.get(Calendar.DAY_OF_MONTH);
    }
    @Bindable
    public String getMonth(){
        Calendar cd = Calendar.getInstance();
        return Common.getMonthName(cd.get(Calendar.MONTH))+".";
    }
    @Bindable
    public int getNoteCount(){
        ArrayList<NoteBook> books = noteProxy.getAllNoteBook();
        return NoteData.noteBooks.size();
    }
    @Bindable
    public int getNotePagesCount(){
        int pages = 0;
        ArrayList<NoteBook> books = noteProxy.getAllNoteBook();
        for(int i=0;i<books.size();i++){
            pages+=books.get(i).pages.size();
        }
        return pages;
    }
}
