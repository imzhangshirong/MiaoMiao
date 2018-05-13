package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.graphics.Point;
import android.hardware.Camera;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

import com.zhangshirong.miaomiao.Base.Debug;

import java.util.List;

import static com.zhangshirong.miaomiao.Common.getCameraInstance;

public class CameraPreview extends FrameLayout {
    private Camera mCamera;
    private CameraView mCameraView;
    private boolean inited = false;


    public CameraPreview(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        mCamera = getCameraInstance();

        // Create our Preview view and set it as the content of our activity.
        mCameraView = new CameraView(getContext(), mCamera);
        this.addView(mCameraView);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        if(mCameraView!=null && !inited){
            RelativeLayout.LayoutParams lp = (RelativeLayout.LayoutParams) getLayoutParams();
            int width = getMeasuredWidth();
            int height = getMeasuredHeight();
            float k = (float)height/(float)width;
            mCameraView.size = findBestPreviewSizeValue(mCamera.getParameters().getSupportedPreviewSizes(),k);
            int height2 = width*mCameraView.size.x/mCameraView.size.y;
            Debug.log(width+"//////////"+height2);
            lp.width = width;
            lp.height = height2;
            setLayoutParams(lp);
            inited=true;
        }
    }

    private Point findBestPreviewSizeValue(List<Camera.Size> sizeList,float ratioK) {
        int bestX = 0;
        int bestY = 0;
        int size = 0;
        for(int i = 0; i < sizeList.size(); i ++){
            // 如果有符合的分辨率，则直接返回
            /*if(sizeList.get(i).width == DEFAULT_WIDTH && sizeList.get(i).height == DEFAULT_HEIGHT){
                Log.d(TAG, "get default preview size!!!");
                return new Point(DEFAULT_WIDTH, DEFAULT_HEIGHT);
            }*/

            int newX = sizeList.get(i).width;
            int newY = sizeList.get(i).height;
            int newSize = Math.abs(newX * newX) + Math.abs(newY * newY);
            float ratio = (float)newX / (float)newY;
            if (newSize >= size && ratio <= ratioK) {
                bestX = newX;
                bestY = newY;
                size = newSize;
            } else if (newSize < size) {
                continue;
            }
        }

        if (bestX > 0 && bestY > 0) {
            return new Point(bestX, bestY);
        }
        return null;
    }
}
