package com.zhangshirong.miaomiao.Base;

import android.util.Log;

/**
 * Created by kyarvis on 18-4-15.
 */

public class Debug {
    public static void log(Object message){
        Log.d("DebugLog",message.toString());
    }
    public static void error(Object message){
        Log.e("DebugLog",message.toString());
    }
    public static void warning(Object message){
        Log.w("DebugLog",message.toString());
    }
}
