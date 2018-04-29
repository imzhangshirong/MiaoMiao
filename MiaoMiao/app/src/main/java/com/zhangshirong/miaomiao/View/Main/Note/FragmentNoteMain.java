package com.zhangshirong.miaomiao.View.Main.Note;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.Base.View.NFragment;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.FragmentPager;
import com.zhangshirong.miaomiao.Widget.FragmentPagerIndicator;

import java.util.ArrayList;

/**
 * Created by kyarvis on 18-4-26.
 */

public class FragmentNoteMain extends NFragment {
    private View fragmentView;

    private FragmentPager viewFragmentPager;
    private FragmentPagerIndicator viewFragmentPagerIndicator;
    private ArrayList<Fragment> fragments = new ArrayList<Fragment>();


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceStat) {
        fragmentView = inflater.inflate(R.layout.fragment_main_note, container, false);


        initView();


        return fragmentView;
    }

    private void initView(){
        viewFragmentPagerIndicator = fragmentView.findViewById(R.id.main_note_main_pager_indicator);
        viewFragmentPager = fragmentView.findViewById(R.id.main_note_main_pager);
        fragments.add(new FragmentIndex());
        fragments.add(new FragmentNotebook());
        fragments.add(new FragmentTest());
        viewFragmentPager.init(this,viewFragmentPagerIndicator,fragments);
    }


    @Override
    public void onStart() {
        super.onStart();
    }

}
