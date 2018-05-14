package com.zhangshirong.miaomiao.DataObject.Struct;

import java.util.ArrayList;

public class NoteBookPage {
    public long bookId;
    public long pageId;
    public ArrayList<NoteBookPageRow> rows = new ArrayList<NoteBookPageRow>();
}
