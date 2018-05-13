package com.zhangshirong.miaomiao.Proxy;

import com.zhangshirong.miaomiao.Base.NProxy;
import com.zhangshirong.miaomiao.DataObject.NoteData;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;

import java.util.ArrayList;

public class NoteProxy extends NProxy {
    @Override
    public void onRemove() {
        super.onRemove();
    }

    @Override
    public void onRegister() {
        super.onRegister();
    }

    public ArrayList<NoteBook> getAllNoteBook(){
        return NoteData.noteBooks;
    }
}
