package com.zhangshirong.miaomiao.Proxy;

import com.zhangshirong.miaomiao.Base.NProxy;
import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;
import com.zhangshirong.miaomiao.DataObject.TestData;

import java.util.ArrayList;

public class TestProxy extends NProxy {
    @Override
    public void onRemove() {
        super.onRemove();
    }

    @Override
    public void onRegister() {
        super.onRegister();
    }

    public ArrayList<TestResult> getAllTestResult(){
        return TestData.testResults;
    }
}
