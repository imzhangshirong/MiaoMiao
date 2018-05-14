package com.zhangshirong.miaomiao.DataObject.Struct;

import java.util.ArrayList;

public class NoteBook {
    public String name;
    public long bookId;
    public String coverImg;
    public long createTime;
    public long updateTime;
    public long lastOpenTime;
    public int fillableCount;
    public ArrayList<NoteBookPage> pages = new ArrayList<NoteBookPage>();
}
