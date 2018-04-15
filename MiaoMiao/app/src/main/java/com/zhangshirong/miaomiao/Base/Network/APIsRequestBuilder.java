package com.zhangshirong.miaomiao.Base.Network;

import java.util.HashMap;

import okhttp3.MultipartBody;
import okhttp3.RequestBody;

/**
 * Created by kyarvis on 18-4-15.
 */

public class APIsRequestBuilder {
    private HashMap<String,Object> formData = new HashMap<String,Object>();
    public void setData(String key,Object data){
        formData.put(key,data);
    }

    public String buildGetURL(String url){
        String reUrl = url;
        reUrl+="?";
        boolean hasFirst = false;
        for(String key: formData.keySet()){
            if(hasFirst)reUrl+="&";
            reUrl+=key+"="+formData.get(key).toString();
            hasFirst=true;
        }
        return reUrl;
    }

    public RequestBody buildPostRequest(){
        MultipartBody.Builder reqb=new MultipartBody.Builder().setType(MultipartBody.FORM);
        for(String key: formData.keySet()){
            reqb.addFormDataPart(key,formData.get(key).toString());
        }
        RequestBody req = reqb.build();
        return req;
    }
}
