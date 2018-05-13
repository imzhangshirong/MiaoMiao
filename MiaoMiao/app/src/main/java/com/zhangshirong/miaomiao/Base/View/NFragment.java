package com.zhangshirong.miaomiao.Base.View;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Widget.Iconfont;

/**
 * Created by kyarvis on 18-4-15.
 */

public class NFragment extends Fragment {
    public Context context;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        context = getContext();
        View view = super.onCreateView(inflater, container, savedInstanceState);
        Iconfont.apply(view, MiaoMiao.iconfont);
        return view;
    }

    public void startActivity(Class activity){
        startActivity(activity,null);
    }

    public void startActivity(Class activity,Bundle bundle){
        Intent mainIntent = new Intent(context,activity);
        if(bundle!=null)mainIntent.putExtras(bundle);
        context.startActivity(mainIntent);
    }
    public void endActivity(Class activity){
        if(NFragmentActivity.activityMap.containsKey(activity)){
            if(NFragmentActivity.activityMap.get(activity)!=null){
                NFragmentActivity.activityMap.get(activity).finish();
            }
            else {
                NFragmentActivity.activityMap.remove(activity);
            }
        }
        if(NActivity.activityMap.containsKey(activity)){
            if(NActivity.activityMap.get(activity)!=null){
                NActivity.activityMap.get(activity).finish();
            }
            else {
                NActivity.activityMap.remove(activity);
            }
        }
    }
}
