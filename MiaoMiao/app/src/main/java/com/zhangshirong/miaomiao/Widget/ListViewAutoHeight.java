package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListAdapter;
import android.widget.ListView;

/**
 * Created by Jarvis on 2016/8/23.
 */
public class ListViewAutoHeight extends ListView{
    public ListViewAutoHeight(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ListViewAutoHeight(Context context) {
        super(context);
    }

    public ListViewAutoHeight(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }


    @Override
    public void setAdapter(ListAdapter adapter) {
        super.setAdapter(adapter);
        new Thread(new Runnable() {
            @Override
            public void run() {
                resetHeight();
            }
        }).start();
    }
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2,MeasureSpec.AT_MOST);
        super.onMeasure(widthMeasureSpec, expandSpec);

    }
    private void resetHeight(){
        final ListAdapter adapter=getAdapter();
        if(adapter!=null){

            post(new Runnable() {
                @Override
                public void run() {
                    int totalHeight = 0;
                    for (int i = 0; i < adapter.getCount(); i++) {
                        View listItem = adapter.getView(i, null, ListViewAutoHeight.this);
                        listItem.measure(0, 0);
                        totalHeight += listItem.getMeasuredHeight();

                    }
                    ViewGroup.LayoutParams params = ListViewAutoHeight.this.getLayoutParams();
                    params.height = totalHeight + (ListViewAutoHeight.this.getDividerHeight() * (ListViewAutoHeight.this.getCount()-1));
                    ListViewAutoHeight.this.setLayoutParams(params);
                }
            });
        }
    }
}
