package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;


import java.util.List;

/**
 * Created by jarvis on 17-4-8.
 */

public class FunctionsPagerIndicator extends PagerIndicator {
    public FunctionsPagerIndicator(Context context) {
        super(context);
    }

    public FunctionsPagerIndicator(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public FunctionsPagerIndicator(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    public void select(int position) {
        reset();
        if(getChildCount()>position){
            View tempView=getChildAt(position);
            //tempView.setBackgroundDrawable(getResources().getDrawable(R.drawable.bk_functions_pop_selected));
        }
        super.select(position);
    }
    private void reset(){
        for(int a=0;a<getChildCount();a++){
            View tempView=getChildAt(a);
            //tempView.setBackgroundDrawable(getResources().getDrawable(R.drawable.bk_functions_pop));
        }
    }
    @Override
    public void setData(List list) {
        LinearLayout.LayoutParams lp=new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        float d = getResources().getDisplayMetrics().density;
        lp.setMargins((int)(3*d),0,(int)(3*d),0);
        lp.width=(int)(6*d);
        lp.height=(int)(6*d);
        for(int a=0;a<list.size();a++){
            View tempView=new View(getContext());
            tempView.setClickable(false);
            //tempView.setBackgroundDrawable(getResources().getDrawable(R.drawable.bk_functions_pop));
            tempView.setLayoutParams(lp);
            this.addView(tempView);
        }
        select(0);
        super.setData(list);
    }
}
