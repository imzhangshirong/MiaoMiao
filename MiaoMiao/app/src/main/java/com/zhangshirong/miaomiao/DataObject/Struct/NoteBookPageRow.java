package com.zhangshirong.miaomiao.DataObject.Struct;

import java.util.ArrayList;

public class NoteBookPageRow{
    public long pageId;
    public long rowId;
    public ArrayList<NoteBookPageRowBlock> blocks = new ArrayList<NoteBookPageRowBlock>();
}
