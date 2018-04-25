package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v4.widget.SwipeRefreshLayout;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.LinearLayout;


/**
 * Created by Jarvis on 2016/8/18.
 */
public class BannerIndicator extends LinearLayout{
    private SwipeRefreshLayout swipe=null;
    private BannerIndicator vpl;
    private ViewPager vp;
    private Runnable touchingCall;
    private Runnable touchEndCall;
    public BannerIndicator(Context context, AttributeSet attrs) {
        super(context, attrs);
        vpl=this;
    }
    public void setOnTouching(Runnable run){
        touchingCall=run;
    }
    public void setOnTouchEnd(Runnable run){
        touchEndCall=run;
    }
    public void init(ViewPager vp_, final SwipeRefreshLayout swipe_){
        swipe=swipe_;
        vp=vp_;
        PagerAdapter vpAdapter= vp.getAdapter();
        LayoutParams lp=new LayoutParams(
                LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT
        );
        float d = getResources().getDisplayMetrics().density;
        lp.setMargins((int)(3*d),0,(int)(3*d),0);
        lp.width=(int)(8*d);
        lp.height=(int)(8*d);
        int N=vpAdapter.getCount();
        if(N>1){
            N=N-2;
        }
        int a;
        this.removeAllViews();
        for(a=0;a<N;a++){
            View tempView=new View(getContext());
            tempView.setClickable(false);
            //tempView.setBackgroundDrawable(getResources().getDrawable(R.drawable.bk_banner_pop));
            tempView.setLayoutParams(lp);
            this.addView(tempView);
        }
        vp.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if(motionEvent.getAction()==MotionEvent.ACTION_UP || motionEvent.getAction()==MotionEvent.ACTION_CANCEL){
                    if(swipe!=null)swipe.setEnabled(true);
                    if(touchEndCall!=null)touchEndCall.run();
                }
                else{
                    if(swipe!=null && !swipe.isRefreshing())swipe.setEnabled(false);
                    if(touchingCall!=null)touchingCall.run();
                }
                return false;
            }
        });
        vp.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
            }
            @Override
            public void onPageSelected(int position) {
                int po=position;
                int N=vpl.getChildCount();
                if(N>1){
                    N=N+2;
                    if (position<1){
                        po=N-3;
                    }
                    else if(position==N-1){
                        po=0;
                    }
                    else{
                        po=position-1;
                    }
                }

                setDefualt();
                View tempView=(View)vpl.getChildAt(po);
                //tempView.setBackgroundDrawable(getResources().getDrawable(R.drawable.bk_banner_pop_selected));
            }

            @Override
            public void onPageScrollStateChanged(int state) {
                if(state==0){
                    int N=vpl.getChildCount();
                    int position=vp.getCurrentItem();
                    if(N>1){
                        N=N+2;
                        if (position<1){
                            vp.setCurrentItem(N-2,false);
                        }
                        else if(position==N-1){
                            vp.setCurrentItem(1,false);
                        }
                    }
                }
            }
        });
        vp.setCurrentItem(1);
    }
    private void setDefualt(){
        int a;
        for(a=0;a<vpl.getChildCount();a++){
            View tempView=(View)vpl.getChildAt(a);
            //tempView.setBackgroundDrawable(getResources().getDrawable(R.drawable.bk_banner_pop));
        }
    }
}
