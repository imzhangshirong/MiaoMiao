package com.zhangshirong.miaomiao.Base;

import android.util.Log;

/**
 * Created by kyarvis on 18-4-15.
 */

public class Debug {
    public static void Log(Object message){
        Log.d("DebugLog",message.toString());
    }
    public static void Error(Object message){
        Log.e("DebugLog",message.toString());
    }
    public static void Warning(Object message){
        Log.w("DebugLog",message.toString());
    }
}
