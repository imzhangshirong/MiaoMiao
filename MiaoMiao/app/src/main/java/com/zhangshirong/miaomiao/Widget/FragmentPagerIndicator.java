package com.zhangshirong.miaomiao.Widget;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;
import android.widget.LinearLayout;


/**
 * Created by Jarvis on 2016/8/24.
 */
public class FragmentPagerIndicator extends PagerIndicator{
    private Path blockPath;
    private Paint blockPaint;
    private int blockPadding;
    private int blockX=0;
    private int blockHeight;
    private int index=0;
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        setBlock(index,0);
    }

    public FragmentPagerIndicator(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView(Color.WHITE);

    }
    private void initView(int color){
        blockPaint=new Paint();
        blockPaint.setAntiAlias(true);
        blockPaint.setColor(color);
        blockPaint.setStyle(Paint.Style.FILL);
        blockHeight= (int) (3*getResources().getDisplayMetrics().density);
        //blockPadding= (int) (15*app.getResources().getDisplayMetrics().density);
        blockPadding=0;
        for(int a=0;a<this.getChildCount();a++){
            this.getChildAt(a).setClickable(true);
            this.getChildAt(a).setFocusable(true);
            this.getChildAt(a).setFocusableInTouchMode(true);
        }

    }
    private void setBlock(int position, float positionOffset){
        float width=this.getChildAt(position).getWidth();
        if(position+1<this.getChildCount()){
            width=this.getChildAt(position+1).getWidth();
        }
        blockPath=new Path();
        blockPath.moveTo(blockPadding,0);
        blockPath.lineTo(width-blockPadding,0);
        blockPath.lineTo(width-blockPadding,blockHeight);
        blockPath.lineTo(blockPadding,blockHeight);
        blockPath.close();
        blockX=0;
        int a;
        for(a=0;a<position;a++){
            blockX+=this.getChildAt(a).getWidth();
        }
        for(a=0;a<getChildCount();a++){
            this.getChildAt(a).setAlpha(0.4f);
        }
        this.getChildAt(position).setAlpha(1f);
        blockX+=positionOffset*width;
    }

    /**
     * 跟随移动
     * @param position
     * @param positionOffset
     */
    @Override
    public void scroll(int position, float positionOffset){
        index=position;
        setBlock(position, positionOffset);
        invalidate();
    }
    @Override
    protected void dispatchDraw(Canvas canvas) {
        //setBlock(index,0);
        canvas.save();
        canvas.translate(blockX,this.getHeight()-blockHeight-2);
        canvas.drawPath(blockPath,blockPaint);
        canvas.restore();
        super.dispatchDraw(canvas);
    }
}
