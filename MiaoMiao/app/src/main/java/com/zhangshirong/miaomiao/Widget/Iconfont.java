package com.zhangshirong.miaomiao.Widget;

import android.app.Activity;
import android.content.Context;
import android.graphics.Typeface;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;


import com.zhangshirong.miaomiao.Base.Debug;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by jarvis on 17-2-17.
 */

public class Iconfont {

    public static Typeface getIconfontTypeface(Context context,String fontAssetPath){
        return Typeface.createFromAsset(context.getApplicationContext().getAssets(), fontAssetPath);
    }
    public static void init(final Activity activity, final Typeface typeface){
        List<View> views = new ArrayList<View>();
        ViewGroup content = (ViewGroup) activity.findViewById(android.R.id.content);
        addViews(content,views);
        for(int a=0;a<views.size();a++){
            try{
                initTextViewIcon(views.get(a),typeface);
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }

    }
    public static void apply(final View view, final Typeface typeface){
        List<View> views = new ArrayList<View>();
        addViews(view,views);
        for(int a=0;a<views.size();a++){
            try{
                initTextViewIcon(views.get(a),typeface);
            }
            catch (Exception e){
                e.printStackTrace();
            }
        }
    }
    private static void initTextViewIcon(View view, Typeface typeface){
        try{
            ((TextView)view).setTypeface(typeface);
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }
    private static void addViews(View content, List<View> views)
    {
        View view = getView(content);
        if (view != null) views.add(view);

        if (content instanceof ViewGroup)
        {
            ViewGroup container = (ViewGroup) content;

            for (int i = 0, n = container.getChildCount(); i < n; i++)
            {
                View child = container.getChildAt(i);
                addViews(child, views);
            }
        }

    }
    private static View getView(View view)
    {
        Object tag = view.getTag();
        if (tag == null) return null;
        if (!(tag instanceof String)) return null;
        String tagStr = (String) tag;
        if(tagStr.equals("iconfont")){
            return view;
        }
        return null;
    }
}
