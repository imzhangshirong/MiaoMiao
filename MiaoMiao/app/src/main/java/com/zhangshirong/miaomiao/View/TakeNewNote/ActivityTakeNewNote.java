package com.zhangshirong.miaomiao.View.TakeNewNote;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.zhangshirong.miaomiao.Base.Debug;
import com.zhangshirong.miaomiao.Base.View.NActivity;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.CameraPreview;
import com.zhangshirong.miaomiao.Widget.CameraView;

import static com.zhangshirong.miaomiao.Common.getCameraInstance;

public class ActivityTakeNewNote extends NActivity implements View.OnClickListener{
    private ImageView viewPreview;
    private Button viewDeal;
    private String imagePath;
    private Bitmap imageData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_takenewnote);
        Bundle bundle = getIntent().getExtras();
        imagePath = bundle.getString("data");
        imageData = BitmapFactory.decodeFile(imagePath);

        initView();
    }

    private void initView(){
        viewPreview = findViewById(R.id.takenewnote_preview);
        viewDeal = findViewById(R.id.takenewnote_deal);
        viewDeal.setOnClickListener(this);
    }

    private void initData(){
        if(imageData!=null){
            viewPreview.setImageBitmap(imageData);
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        initData();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.takenewnote_deal:
                Toast.makeText(context,imageData.getWidth()+","+imageData.getHeight(),Toast.LENGTH_SHORT).show();
                break;
        }
    }
}
