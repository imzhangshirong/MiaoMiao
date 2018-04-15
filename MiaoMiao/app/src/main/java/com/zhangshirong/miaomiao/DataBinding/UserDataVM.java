package com.zhangshirong.miaomiao.DataBinding;

import com.zhangshirong.miaomiao.Base.ViewModel.ViewModel;

/**
 * Created by kyarvis on 18-4-15.
 */

public class UserDataVM extends ViewModel {
    private String greeting;
    public String getGreeting(){
        return "Helloooooo";
    }
    public void setGreeting(String greeting){
        this.greeting=greeting;
    }
}
