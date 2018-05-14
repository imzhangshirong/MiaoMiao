package com.zhangshirong.miaomiao.DataObject;


import com.zhangshirong.miaomiao.DataObject.Struct.TestResult;

import java.util.ArrayList;

public class TestData {
    public static ArrayList<TestResult> testResults = new ArrayList<TestResult>();
    public static void mockData(){
        TestResult result = new TestResult();
        result.bookId = 1;
        result.name = "高等代数";
        result.curPosition = 10;
        result.endPosition = 16;
        result.allFillBlock = 60;
        result.correctFillBlock = 40;
        result.createTime = 1526275850;
        result.updateTime = 1526276723;
        testResults.add(result);
        result = new TestResult();
        result.bookId = 2;
        result.name = "计算机组成原理";
        result.curPosition = 16;
        result.endPosition = 16;
        result.allFillBlock = 50;
        result.correctFillBlock = 47;
        result.createTime = 1526275850;
        result.updateTime = 1526277410;
        result.endTime = 1526277410;
        testResults.add(result);
    }
}
