package com.zhangshirong.miaomiao.DataObject.Struct;

import java.util.ArrayList;

public class NoteBook {
    public String name;
    public int bookId;
    public String coverImg;
    public int createTime;
    public int updateTime;
    public int lastOpenTime;
    public int fillableCount;
    public ArrayList<NoteBookPage> pages = new ArrayList<NoteBookPage>();
}
