package com.zhangshirong.miaomiao.Base.View;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Icon;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;

import com.zhangshirong.miaomiao.Base.Debug;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.Iconfont;

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
        View view = ((ViewGroup)findViewById(android.R.id.content)).getChildAt(0);
        Iconfont.apply(view, MiaoMiao.iconfont);
        super.onStart();
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
