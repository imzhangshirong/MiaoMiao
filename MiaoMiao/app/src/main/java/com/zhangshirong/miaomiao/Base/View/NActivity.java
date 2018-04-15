package com.zhangshirong.miaomiao.Base.View;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import com.zhangshirong.miaomiao.Base.Debug;

import java.util.HashMap;

/**
 * Created by kyarvis on 18-4-15.
 */

public class NActivity extends Activity {
    protected static HashMap<Class,NActivity> activityMap = new HashMap<Class,NActivity>();
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
        if(NFragmentActivity.activityMap.containsKey(activity)){
            if(NFragmentActivity.activityMap.get(activity)!=null){
                NFragmentActivity.activityMap.get(activity).finish();
            }
            else {
                NFragmentActivity.activityMap.remove(activity);
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
