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

    public NoteBook getNoteBookById(long id){
        NoteBook book = null;
        for(int i=0;i<NoteData.noteBooks.size();i++){
            NoteBook temp = NoteData.noteBooks.get(i);
            if(temp.bookId == id){
                book = temp;
                break;
            }
        }
        return book;
    }
}
