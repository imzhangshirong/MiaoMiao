package com.zhangshirong.miaomiao.DataBinding;

import android.databinding.Bindable;

import com.zhangshirong.miaomiao.Base.ViewModel.ViewModel;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.Proxy.UserProxy;

public class MainMeVM extends ViewModel {
    private UserProxy userProxy;
    public MainMeVM(){
        userProxy = (UserProxy) MiaoMiao.getProxy(UserProxy.class);
    }
    @Bindable
    public String getNickname(){
        return userProxy.getUserNickname();
    }

    @Bindable
    public String getProfileImg(){
        return userProxy.getUserProfileImg();
    }
}
