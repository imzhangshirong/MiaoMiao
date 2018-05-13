package com.zhangshirong.miaomiao.Proxy;

import android.content.Context;
import android.content.SharedPreferences;

import com.zhangshirong.miaomiao.Base.NProxy;
import com.zhangshirong.miaomiao.MiaoMiao;
import com.zhangshirong.miaomiao.R;

/**
 * Created by kyarvis on 18-4-15.
 */

public class LocalDataProxy extends NProxy {
    private SharedPreferences settingSP;
    private SharedPreferences.Editor settingSPEditor;
    private Context context;
    @Override
    public void onRegister() {
        super.onRegister();
        context = MiaoMiao.getInstance().getApplicationContext();
        settingSP = context.getSharedPreferences(context.getString(R.string.SETING_FILE), Context.MODE_PRIVATE);
        settingSPEditor = settingSP.edit();
    }

    @Override
    public void onRemove() {
        super.onRemove();
    }

    public void saveNoteData(){

    }
    public void saveUserData(){

    }
    public void saveTestData(){

    }
    public void saveAPIsData(){

    }
    public void loadNoteData(){

    }
    public void loadUserData(){

    }
    public void loadTestData(){

    }
    public void loadAPIsData(){

    }
}
