package com.zhangshirong.miaomiao.DataObject;

import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBookPage;

import java.util.ArrayList;

public class NoteData {
    public static ArrayList<NoteBook> noteBooks = new ArrayList<NoteBook>();

    public static void mockData(){
        NoteBook book = new NoteBook();
        book.bookId = 1;
        book.coverImg = "null";
        book.createTime = 1526199728;
        book.lastOpenTime = 1526199728;
        book.updateTime = 1526199728;
        book.fillableCount = 145;
        book.name = "高等代数";
        book.coverImg = "http://192.168.1.249/MiaoMiao/cover/cover2.png";
        for(int i=0;i<10;i++)book.pages.add(new NoteBookPage());
        noteBooks.add(book);

        book = new NoteBook();
        book.bookId = 2;
        book.coverImg = "null";
        book.createTime = 1526217728;
        book.lastOpenTime = 1526217728;
        book.updateTime = 1526217728;
        book.fillableCount = 67;
        book.name = "计算机组成原理";
        book.coverImg = "http://192.168.1.249/MiaoMiao/cover/cover3.png";
        for(int i=0;i<5;i++)book.pages.add(new NoteBookPage());
        noteBooks.add(book);

        book = new NoteBook();
        book.bookId = 3;
        book.coverImg = "null";
        book.createTime = 1526217728;
        book.lastOpenTime = 1526217728;
        book.updateTime = 1526217728;
        book.fillableCount = 67;
        book.name = "DEEP LEARNING";
        book.coverImg = "http://192.168.1.249/MiaoMiao/cover/cover4.png";
        for(int i=0;i<32;i++)book.pages.add(new NoteBookPage());
        noteBooks.add(book);

        book = new NoteBook();
        book.bookId = 4;
        book.coverImg = "null";
        book.createTime = 1526217728;
        book.lastOpenTime = 1526217728;
        book.updateTime = 1526217728;
        book.fillableCount = 67;
        book.name = "数学之美";
        book.coverImg = "http://192.168.1.249/MiaoMiao/cover/cover.png";
        for(int i=0;i<20;i++)book.pages.add(new NoteBookPage());
        noteBooks.add(book);
    }
}

