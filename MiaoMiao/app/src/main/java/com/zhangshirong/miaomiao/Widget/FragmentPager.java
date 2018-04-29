package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v4.widget.SwipeRefreshLayout;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by jarvis on 17-2-19.
 */

public class FragmentPager extends ViewPager {
    private ArrayList<Fragment> fragments=new ArrayList<>();
    private SwipeRefreshLayout swipe=null;
    private PagerIndicator pagerIndicator;
    public FragmentPager(Context context) {
        super(context);
    }

    public FragmentPager(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void init(FragmentActivity fragmentActivity, PagerIndicator pagerIndicator){
        init(fragmentActivity,pagerIndicator,new ArrayList<Fragment>());

    }
    public void init(Fragment fragment, PagerIndicator pagerIndicator){
        init(fragment,pagerIndicator,new ArrayList<Fragment>());

    }
    public void init(FragmentActivity fragmentActivity,PagerIndicator pagerIndicator,ArrayList<Fragment> fragments){
        this.pagerIndicator=pagerIndicator;
        this.fragments=fragments;
        initIndicator();
        FragmentManager fm=fragmentActivity.getSupportFragmentManager();
        fm.
        initEvent(fm);
    }
    private void initIndicator(){
        this.pagerIndicator.setData(fragments);
        for(int a=0;a<this.pagerIndicator.getChildCount();a++){
            View view = this.pagerIndicator.getChildAt(a);
            view.setClickable(true);
            view.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View view) {
                    int a;
                    for(a=0;a<FragmentPager.this.pagerIndicator.getChildCount();a++){
                        View view_ = FragmentPager.this.pagerIndicator.getChildAt(a);
                        if(view.equals(view_)){
                            break;
                        }
                    }
                    FragmentPager.this.setCurrentItem(a);
                }
            });
        }
    }
    public void setSwipe(SwipeRefreshLayout swipe){
        this.swipe=swipe;
    }
    private void initEvent(FragmentManager fm){
        FragmentPagerAdapter fpa=new FragmentPagerAdapter(fm) {
            @Override
            public void destroyItem(ViewGroup container, int position, Object object) {
                //container.removeView((View) object);
                //super.destroyItem(container, position, object);
            }

            @Override
            public Fragment getItem(int position) {
                return FragmentPager.this.fragments.get(position);
            }


            @Override
            public int getCount() {
                return FragmentPager.this.fragments.size();
            }
        };
        setAdapter(fpa);
        fpa.
        addOnPageChangeListener(new OnPageChangeListener() {
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
                FragmentPager.this.pagerIndicator.scroll(position,positionOffset);
            }

            @Override
            public void onPageSelected(int position) {
                FragmentPager.this.pagerIndicator.select(position);
            }

            @Override
            public void onPageScrollStateChanged(int state) {

            }
        });
        setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if(swipe!=null){
                    if(motionEvent.getAction()==1 || motionEvent.getAction()==3){
                        swipe.setEnabled(true);
                    }
                    else{
                        if(!swipe.isRefreshing())swipe.setEnabled(false);
                    }
                }
                return false;
            }
        });
    }
    public void init(Fragment fragment,PagerIndicator pagerIndicator,ArrayList<Fragment> fragments){
        this.pagerIndicator=pagerIndicator;
        this.fragments=fragments;
        initIndicator();
        FragmentManager fm=fragment.getChildFragmentManager();//注意这里用getFragmentManager在再次加载父Fragment后会导致Fragment初始化失败
        initEvent(fm);
    }

    public void refresh(){
        getAdapter().notifyDataSetChanged();
    }
}
