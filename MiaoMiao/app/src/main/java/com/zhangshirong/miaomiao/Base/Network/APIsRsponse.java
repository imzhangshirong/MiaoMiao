package com.zhangshirong.miaomiao.Base.Network;

/**
 * Created by kyarvis on 18-4-15.
 */

public interface APIsRsponse {
    void onResponse(APIsDataObject data);
    void onFailure(APIsDataObject data);
}
