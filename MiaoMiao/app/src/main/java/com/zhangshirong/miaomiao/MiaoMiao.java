package com.zhangshirong.miaomiao;

import android.app.Application;
import com.zhangshirong.miaomiao.Base.NProxy;
import java.util.HashMap;


/**
 * Created by kyarvis on 18-4-15.
 */

public class MiaoMiao extends Application {
    private static MiaoMiao instance;
    private static HashMap<Class,NProxy> proxyMap = new HashMap<Class,NProxy>();

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
    }

    void registerProxy(){

    }

    public static MiaoMiao getInstance(){
        return instance;
    }

    public static void registerProxy(Class proxy){
        if(proxyMap.containsKey(proxy)){
            proxyMap.get(proxy).onRemove();
            try {
                proxyMap.put(proxy,(NProxy) proxy.newInstance());
            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
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
