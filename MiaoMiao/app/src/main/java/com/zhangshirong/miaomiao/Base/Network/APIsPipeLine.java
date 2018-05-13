package com.zhangshirong.miaomiao.Base.Network;

import android.content.Context;

import com.zhangshirong.miaomiao.Base.Debug;
import com.zhangshirong.miaomiao.Config;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.HashMap;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

/**
 * Created by kyarvis on 18-4-15.
 */

public class APIsPipeLine {
    private static APIsPipeLine instance;
    public static APIsPipeLine getInstance(){
        if(instance==null)instance = new APIsPipeLine();
        return instance;
    }

    public APIsDataObject requestSync(String APIUrl,APIsRequestBuilder urlData,APIsRequestBuilder formData){
        String url=urlData.buildGetURL(APIUrl);
        Debug.log(url);
        APIsDataObject apiData = new APIsDataObject();
        OkHttpClient mOkHttpClient = new OkHttpClient.Builder()
                .connectTimeout(Config.API_CONNECT_TIMEOUT, TimeUnit.MILLISECONDS)
                .readTimeout(Config.API_READ_TIMEOUT,TimeUnit.MILLISECONDS)
                .build();
        Request request = new Request.Builder()
                .url(url)
                .post(formData.buildPostRequest())
                .build();
        try {
            Response response = mOkHttpClient.newCall(request).execute();
            switch (response.code()){
                case 200:
                case 301:
                    apiData.session = response.header("session");
                    String data=response.body().string();
                    Debug.log(data);
                    JSONObject jsonData = new JSONObject(data);
                    apiData.code = jsonData.getInt("code");
                    apiData.message = jsonData.getString("msg");
                    apiData.jsonData = jsonData;
                    break;
                default:
                    apiData.code = -1;
                    break;
            }

        } catch (IOException e) {
            Debug.error(e.getMessage());
            e.printStackTrace();
        }
        finally {
            return apiData;
        }
    }


    public void requestAsync(String APIUrl,final APIsRsponse apiCallBack, APIsRequestBuilder urlData, APIsRequestBuilder formData){
        final String url=urlData.buildGetURL(APIUrl);
        Debug.log(url);
        final APIsDataObject apiData = new APIsDataObject();
        OkHttpClient mOkHttpClient = new OkHttpClient.Builder()
                .connectTimeout(Config.API_CONNECT_TIMEOUT, TimeUnit.MILLISECONDS)
                .readTimeout(Config.API_READ_TIMEOUT,TimeUnit.MILLISECONDS)
                .build();
        Request request = new Request.Builder()
                .url(url)
                .post(formData.buildPostRequest())
                .build();

        final Call call = mOkHttpClient.newCall(request);
        call.enqueue(new Callback()
        {
            @Override
            public void onFailure(Call call, IOException e) {
                apiData.code = -1;
                apiData.message = e.getMessage();
                apiCallBack.onFailure(apiData);
            }
            @Override
            public void onResponse(Call call, Response response) throws IOException {
                apiData.session = response.header("session");
                String data=response.body().string();
                Debug.log(data);
                JSONObject jsonData = null;
                try {
                    jsonData = new JSONObject(data);
                    apiData.code = jsonData.getInt("code");
                    apiData.message = jsonData.getString("msg");
                    apiData.jsonData = jsonData;
                } catch (JSONException e) {
                    apiData.code = -2;
                    Debug.error(e.getMessage());
                    e.printStackTrace();
                }
                if(apiData.code!=0){
                    apiCallBack.onFailure(apiData);
                }
                else {
                    apiCallBack.onResponse(apiData);
                }
            }
        });
    }

}
