package com.zhangshirong.miaomiao.View.TakeNewNote;

import android.hardware.Camera;
import android.os.Bundle;
import android.widget.FrameLayout;

import com.zhangshirong.miaomiao.Base.View.NActivity;
import com.zhangshirong.miaomiao.R;
import com.zhangshirong.miaomiao.Widget.CameraView;

import static com.zhangshirong.miaomiao.Common.getCameraInstance;

public class ActivityTakeNewNote extends NActivity {

    private Camera mCamera;
    private CameraView mPreview;
    private FrameLayout viewCamera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_takenewnote);

        initView();
    }

    private void initView(){
        mCamera = getCameraInstance();

        // Create our Preview view and set it as the content of our activity.
        mPreview = new CameraView(this, mCamera);
        viewCamera = (FrameLayout) findViewById(R.id.takenewnote_camera_view);
        viewCamera.addView(mPreview);
    }

    @Override
    protected void onStart() {
        super.onStart();
    }
}
