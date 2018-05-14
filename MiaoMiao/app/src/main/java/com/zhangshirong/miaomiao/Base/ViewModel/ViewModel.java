package com.zhangshirong.miaomiao.Base.ViewModel;

import android.databinding.BaseObservable;
import android.databinding.BindingAdapter;
import android.graphics.Bitmap;
import android.view.View;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.ListView;

import com.makeramen.roundedimageview.RoundedImageView;
import com.squareup.picasso.Picasso;
import com.zhangshirong.miaomiao.Base.Debug;
import com.zhangshirong.miaomiao.Widget.ListAdapter;
import com.zhangshirong.miaomiao.Widget.ListViewAutoHeight;

/**
 * Created by kyarvis on 18-4-15.
 */

public class ViewModel extends BaseObservable {
    @BindingAdapter({"adapter"})
    public static void bindList(ListView view, ListAdapter adapter) {
        view.setAdapter(adapter);
    }

    @BindingAdapter({"adapter"})
    public static void bindGrid(GridView view, ListAdapter adapter) {
        view.setAdapter(adapter);
    }

    @BindingAdapter("android:src")
    public static void setSrc(ImageView view, String url) {
        Picasso.get().load(url).config(Bitmap.Config.RGB_565).into(view);
    }
    @BindingAdapter("android:src")
    public static void setSrc(RoundedImageView view, String url) {
        Picasso.get().load(url).config(Bitmap.Config.RGB_565).into(view);
    }

}
