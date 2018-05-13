package com.zhangshirong.miaomiao.DataObject;


import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;

import java.util.ArrayList;

public class TestData {
    public static ArrayList<TestResult> testResults = new ArrayList<TestResult>();
    public static void mockData(){
        TestResult result = new TestResult();
        result.name = "高等代数";
        result.curPosition = 10;
        result.endPosition = 16;
        testResults.add(result);
        result = new TestResult();
        result.name = "计算机组成原理";
        result.curPosition = 14;
        result.endPosition = 16;
        testResults.add(result);
    }
}
