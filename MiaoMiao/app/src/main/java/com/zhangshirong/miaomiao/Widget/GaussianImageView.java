package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.widget.ImageView;

import com.zhangshirong.miaomiao.Common;


/**
 * Created by jarvis on 16-10-12.
 */
public class GaussianImageView extends ImageView {
    private Bitmap curr=null;
    private Drawable currDrawable=null;
    public GaussianImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
        if(getDrawable()!=null){
            setImageBitmap(((BitmapDrawable)getDrawable()).getBitmap());
        }
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);

    }
    private static Bitmap scale(Bitmap bitmap,float size) {
        Matrix matrix = new Matrix();
        matrix.postScale(size,size); //长和宽放大缩小的比例
        Bitmap resizeBmp = Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),matrix,true);
        return resizeBmp;
    }

    @Override
    public void setImageDrawable(Drawable drawable) {
        if(drawable!=null && currDrawable!=null && currDrawable.equals(drawable)){
            Animation animation = new AlphaAnimation(0, 1);
            animation.setDuration(200);
            this.setAnimation(animation);
            super.setImageDrawable(drawable);
        }
        else if(drawable!=null){
            curr=((BitmapDrawable)drawable).getBitmap();
            if(curr!=null){
                curr=scale(curr,0.15f);
                gaussian();
            }
            else{
                super.setImageDrawable(drawable);
            }
        }
        else{
            super.setImageDrawable(drawable);
        }

    }

    private void gaussian(){
        if(curr!=null){
            currDrawable=new BitmapDrawable(Common.doBlur(curr,4,false));
            curr.recycle();
            curr=null;
            post(new Runnable() {
                @Override
                public void run() {
                    GaussianImageView.this.setImageDrawable(currDrawable);
                }
            });
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
    }

}
