package com.zhangshirong.miaomiao.Proxy;

import com.zhangshirong.miaomiao.Base.NProxy;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.DataObject.TestData;
import com.zhangshirong.miaomiao.DataObject.UserData;

import java.util.ArrayList;

public class UserProxy extends NProxy {
    @Override
    public void onRemove() {
        super.onRemove();
    }

    @Override
    public void onRegister() {
        super.onRegister();
    }

    public String getUserNickname(){
        return UserData.nickName;
    }
    public String getUserProfileImg(){
        return UserData.profileImg;
    }
}
