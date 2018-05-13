package com.zhangshirong.miaomiao;

import android.app.Application;
import android.graphics.Typeface;

import com.zhangshirong.miaomiao.Base.Debug;
import com.zhangshirong.miaomiao.Base.NProxy;
import com.zhangshirong.miaomiao.DataObject.NoteData;
import com.zhangshirong.miaomiao.DataObject.Struct.NoteBook;
import com.zhangshirong.miaomiao.DataObject.TestData;
import com.zhangshirong.miaomiao.DataObject.UserData;
import com.zhangshirong.miaomiao.Proxy.APIsProxy;
import com.zhangshirong.miaomiao.Proxy.LocalDataProxy;
import com.zhangshirong.miaomiao.Proxy.NoteProxy;
import com.zhangshirong.miaomiao.Widget.Iconfont;

import java.util.HashMap;


/**
 * Created by kyarvis on 18-4-15.
 */

public class MiaoMiao extends Application {
    private static MiaoMiao instance;
    private static HashMap<Class,NProxy> proxyMap = new HashMap<Class,NProxy>();

    public static Typeface iconfont;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        initData();

    }

    void initData(){
        //View Init
        this.iconfont = Iconfont.getIconfontTypeface(this,Config.APP_ICONFONT);
        //Mock Data
        mockData();
        //Proxy
        registerProxy();
    }

    void registerProxy(){
        registerProxy(LocalDataProxy.class);
        registerProxy(APIsProxy.class);
        registerProxy(NoteProxy.class);
    }

    void mockData(){
        UserData.mockData();
        NoteData.mockData();
        TestData.mockData();
    }

    public static MiaoMiao getInstance(){
        return instance;
    }

    public static void registerProxy(Class proxy){
        if(proxyMap.containsKey(proxy)){
            removeProxy(proxy);
        }
        try {
            NProxy proxyIns = (NProxy) proxy.newInstance();
            proxyMap.put(proxy,proxyIns);
            proxyIns.onRegister();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
    }
    public static void removeProxy(Class proxy){
        if(proxyMap.containsKey(proxy)){
            proxyMap.get(proxy).onRemove();
            proxyMap.remove(proxy);
        }
    }
    public static NProxy getProxy(Class proxy){
        if(proxyMap.containsKey(proxy)){
            return proxyMap.get(proxy);
        }
        return null;
    }

}
