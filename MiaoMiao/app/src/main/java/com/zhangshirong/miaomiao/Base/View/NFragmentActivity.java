package com.zhangshirong.miaomiao.Base.View;

import android.content.Context;
import android.content.Intent;
import android.support.v4.app.FragmentActivity;

import java.util.HashMap;

/**
 * Created by kyarvis on 18-4-15.
 */

public class NFragmentActivity extends FragmentActivity {
    protected static HashMap<Class,NFragmentActivity> activityMap = new HashMap<Class,NFragmentActivity>();
    public Context context;

    @Override
    protected void onStart() {
        activityMap.put(this.getClass(),this);
        this.context = getBaseContext();
        super.onStart();

    }

    public void startActivity(Class activity){
        Intent mainIntent = new Intent(context,activity);
        context.startActivity(mainIntent);
    }
    public void endActivity(Class activity){
        if(activityMap.containsKey(activity)){
            if(activityMap.get(activity)!=null){
                activityMap.get(activity).finish();
            }
            else {
                activityMap.remove(activity);
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

    @Override
    protected void onDestroy() {
        if(activityMap.containsKey(this.getClass())){
            activityMap.remove(this.getClass());
        }

        super.onDestroy();
    }
}
