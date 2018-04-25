package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.LinearLayout;

import java.util.List;

/**
 * Created by jarvis on 17-4-8.
 */

public class PagerIndicator extends LinearLayout {
    private List indicatorData;
    public PagerIndicator(Context context) {
        super(context);
    }

    public PagerIndicator(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public PagerIndicator(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }
    public void scroll(int position, float positionOffset){
    }
    public void select(int position){
    }
    public void setData(List list){
        this.indicatorData=list;
    }
}
